#include "task1.h"
#include "task2.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	int excode;
	struct item *first;
	char **argv;
	do {
		excode = get_line(&first);
		argv = to_argv(first);
		if(!argv)
			continue;
		if(vec_equal(argv[0], "cd")) {
			ch_dir(argv);
		} else {
			execute(argv);
			wait(NULL);
		}
		free_line(first);
		free(argv);
	} while(excode != EOF);
	return 0;
}
