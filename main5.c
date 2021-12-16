#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "task5.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int main()	
{
	int i, cd_perm = 1, w, excode, fg_bg, fd_in, fd_out, pid_i, zombie_i;
	struct item *first;
	char **argv;
	char ***vec_argv;
	int *pid;
	signal(SIGCHLD, chld_hdl);
	do {
		if(geteuid())
			fputs("$ ", stdout);
		else
			fputs("# ", stdout);
		excode = get_line(&first);
		argv = to_argv(first);
		fd_in = pop_redir_in(&argv);
		fd_out = pop_redir_out(&argv);
		fg_bg = pop_bgtag(&argv);
		vec_argv = divide_argv(argv);
		if(!vec_argv || fd_in == r_err || fd_out == r_err || fg_bg == bg_err) {
			goto term;
		}
		if(vec_argv[1])
			cd_perm = 0;
		for(i = 0; vec_argv[i]; i++) {
			if(vec_equal(vec_argv[i][0], "cd")) {
				if(cd_perm && fd_in == r_no && fd_out == r_no && fg_bg == fg)
					ch_dir(vec_argv[i]);
				else
					fputs("cd: Syntax error\n", stderr);
				goto term;
			}
		}
		if(fg_bg == fg) {
			signal(SIGCHLD, SIG_DFL);
			pid = pipe_execute(vec_argv, fd_in, fd_out);
			zombie_i = 0;
			while(pid[zombie_i]) {
				w = wait(NULL);
				for(pid_i = 0; pid[pid_i]; pid_i++) {
					if(w == pid[pid_i]) {
						zombie_i++;
						break;
					}
				}
			}
			zombie_clean(); 
			signal(SIGCHLD, chld_hdl);
		} else {
			pid = pipe_execute(vec_argv, fd_in, fd_out);
			fputs("background: ", stdout);
			for(pid_i = 0; pid[pid_i]; pid_i++) {
				printf("%d ", pid[pid_i]);
			}
			putchar('\n');
		}
		free(pid);
		
term:	if(fd_in != r_no)
			close(fd_in);
		if(fd_out != r_no)
			close(fd_out);
		free_line(first);
		free(vec_argv);
		free(argv);
		cd_perm = 1;
	} while(excode != EOF);
	puts("exit");
	return 0;
}
