#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	int excode, fg_bg, fd_in, fd_out;
	struct item *first;
	char **argv;
	do {
		signal(SIGCHLD, chld_hdl);
		excode = get_line(&first);
		argv = to_argv(first);
		fd_in = pop_redir_in(&argv);
		fd_out = pop_redir_out(&argv);
		fg_bg = pop_bgtag(&argv);
		if(!argv || fd_in == r_err || fd_out == r_err || fg_bg == bg_err) {
			free_line(first);
			free(argv);
			continue;
		}
		if(vec_equal(argv[0], "cd")) {
			ch_dir(argv);
		} else {
			redir_execute(argv, fd_in, fd_out, fg_bg);
		}
		close(fd_in);
		close(fd_out);
		free_line(first);
		free(argv);
	} while(excode != EOF);
	return 0;
}
