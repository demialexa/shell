#include "task1.h"
#include "task3.h"
#include "task4.h"
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int get_argc(char **argv)
{
	int i;
	if(!argv)
		return 0;
	for(i = 0; argv[i]; i++)
		;
	return i;
}

static int redir_open(const char *f_name, int opt)
{
	int fd;
	if(!f_name)
		return r_no;
	switch (opt) {
	case r_in:
		fd = open(f_name, O_RDONLY);
		break;
	case r_out:
		fd = open(f_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		break;
	case r_append:
		fd = open(f_name, O_WRONLY | O_CREAT | O_APPEND, 0666);
	}
	if(fd == -1) {
		perror(f_name);
		return r_err;
	}
	return fd;
}

int pop_redir_in(char ***argv)
{
	int argc, fd_in, i = 0, j = 0;
	char *f_name = NULL;
	char **new_argv;
	if(!*argv)
		return r_no;
	argc = get_argc(*argv);
	new_argv = malloc((argc+1)*sizeof(*new_argv));
	for(i = 0; i <= argc; i++) {
		if((*argv)[i] == REDIR_IN) {
			i++;
			if(f_name || !(*argv)[i] || iftag((*argv)[i])) {
				fputs("<: Syntax error\n", stderr);
				free(new_argv);
				return r_err;
			}
			f_name = (*argv)[i];
		} else {
			new_argv[j++] = (*argv)[i];
		}
	}
	free(*argv);
	*argv = (new_argv[0])? new_argv: NULL;
	if(!f_name)
		return r_no;
	fd_in = redir_open(f_name, r_in);
	return fd_in; 
}

int pop_redir_out(char ***argv)
{
	int argc, fd_out, i = 0, j = 0, append_f = 0;
	char *f_name = NULL;
	char **new_argv;
	if(!*argv)
		return r_no;
	argc = get_argc(*argv);
	new_argv = malloc((argc + 1) * sizeof(*new_argv));
	for(i = 0; i <= argc; i++) {
		if((*argv)[i] == REDIR_OUT || (*argv)[i] == REDIR_APPEND) {
			append_f = (*argv)[i] == REDIR_APPEND? 1: 0;
			i++;
			if(f_name || !(*argv)[i] || iftag((*argv)[i])) {
				fputs(">/>>: Syntax error\n", stderr);
				free(new_argv);
				return r_err;
			}
			f_name = (*argv)[i];
		} else {
			new_argv[j++] = (*argv)[i];
		}
	}
	free(*argv);
	*argv = (new_argv[0])? new_argv: NULL;
	if(!f_name)
		return r_no;
	fd_out = redir_open(f_name, append_f? r_append: r_out);
	return fd_out;
}

void redir(int fd1, int fd2)
{
	if(fd1 != r_no) {
		dup2(fd1, fd2);
		close(fd1);
	}
}


int redir_execute(char * const *argv, int fd_in, int fd_out, int fg_bg)
{
	int w, p, pgid;
	if(fg_bg == fg)
		signal(SIGCHLD, SIG_DFL);
	p = fork();
	if(p == -1) {
		perror("fork");
	} else
	if(p == 0) {
		int pid;
		pid = getpid();
		setpgid(pid, pid);
		printf("child: %d\n", pid);
		tcsetpgrp(0, pid);
		redir(fd_in, 0);
		redir(fd_out, 1);
		execvp(argv[0], argv);
		perror(argv[0]); 
		fflush(stderr);
		_exit(1);
	}
	if(fg_bg == fg) {
		pgid = getpgid(getpid());
		printf("parent: %d\n", pgid);
		while((w = wait(NULL)) != p)
			;
		signal(SIGCHLD, chld_hdl);
		tcsetpgrp(0, pgid);
	} else {
		printf("Background: %d\n", p);
	}
	return p;
}
