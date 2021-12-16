#include "task1.h"
#include "task2.h"
#include "task3.h"
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main()
{
	int excode, fg_bg;
	struct item *first;
	char **argv;
	do {
		signal(SIGCHLD, chld_hdl);
		excode = get_line(&first);
		argv = to_argv(first);
		fg_bg = pop_bgtag(&argv);
		if(!argv || fg_bg == bg_err) {
			free_line(first);
			free(argv);
			continue;
		}
		if(vec_equal(argv[0], "cd")) {
			ch_dir(argv);
		} else {
			int p, w;
			if(fg_bg == fg)
				signal(SIGCHLD, SIG_DFL);
			p = execute(argv);
			if(fg_bg == fg) {
				while((w = wait(NULL)) != p)
					;
				signal(SIGCHLD, chld_hdl);
			} else {
				printf("Background: %d\n", p);
			}
		}
		free_line(first);
		free(argv);
	} while(excode != EOF);
	return 0;
}
