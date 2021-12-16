#include "task1.h"
#include "task4.h"
#include "task5.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

char ***divide_argv(char **argv)
{
	int i, k = 0, argc;
	char ***vec_argv;
	if(!argv)
		return NULL;
	argc = get_argc(argv);
	vec_argv = malloc((argc + 1) * sizeof(*vec_argv));
	vec_argv[k] = argv;
	for(i = 0; argv[i]; i++) {
		if(argv[i] == PIPE) {
			if(!i || !argv[i+1] || argv[i+1] == PIPE) {
				fputs("|: Syntax error\n", stderr);
				free(vec_argv);
				vec_argv = NULL;
				return vec_argv;
			}
			argv[i] = NULL;	
			k++;
			vec_argv[k] = argv + i + 1;
		}
	}
	vec_argv[k+1] = NULL;
	return vec_argv;
}

void child(char **argv, int rd, int wr)
{
	redir(rd, 0);
	redir(wr, 1);
	execvp(argv[0], argv);
	perror(argv[0]);
	fflush(stderr);
	_exit(1);
}	

int* pipe_execute(char ***vec_argv, int fd_in, int fd_out)
{
	int i, k, rd = fd_in, wr;
	int fd[2];
	int *pid;
	for(k = 0; vec_argv[k]; k++)
		;
	pid = malloc((k + 1) * sizeof(*pid));
	for(i = 0; vec_argv[i]; i++) {
		if(vec_argv[i+1])
			pipe(fd);
		wr = vec_argv[i+1]? fd[1]: fd_out;
		pid[i] = fork();
		if(pid[i] == 0) {
			if(vec_argv[i+1]) {
				close(fd[0]);
				if(fd_out != r_no)
					close(fd_out);
			}
			if(i && fd_in != r_no)
				close(fd_in);
			child(vec_argv[i], rd, wr);
		}
		if(i && rd != r_no)
			close(rd);
		if(vec_argv[i]) {
			rd = fd[0];
			if(wr != r_no)
				close(wr);
		}
	}
	pid[i] = 0;
	return pid;
}	
