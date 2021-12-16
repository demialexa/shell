#include "task1.h"
#include "task2.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void free_argv(char ***argv)
{
	int i;
	if(!*argv)
		return;
	for(i = 0; (*argv)[i]; i++) {
		if(!iftag((*argv)[i])) {
			free((*argv)[i]);
		}
	}
	free(*argv);
}

int vec_equal(const char *vec1, const char *vec2)
{
	int i;
	for(i = 0; vec1[i] == vec2[i]; i++) {
		if(!vec1[i])
			return 1;
	}
	return 0;
}

static int li_length(const struct item *first)
{
	int i;
	for(i = 0; first; i++)
		first = first->next;
	return i;
}

char **to_argv(const struct item *first)
{
	char **argv;
	int i, argv_len;
	if(!first)
		return NULL;
	argv_len = li_length(first);
	argv = malloc((argv_len + 1) * sizeof(*argv));
	argv[argv_len] = NULL;
	for(i = argv_len - 1; i >= 0; i--) {
		argv[i] = first->word;
		first = first->next;
	}
	return argv;
}

void ch_dir(char * const *argv)
{
	int res;
	if(!argv[1]) {
		char *home_dir;
		home_dir = getenv("HOME");
		if(!home_dir) {
			fputs("cd: variable HOME is not set", stderr);
		} else {
			res = chdir(home_dir);
			if(res == -1)
				perror(home_dir);
		}
	} else {
		if(argv[2]) { 
			fputs("cd: Too many arguments\n", stderr);
		} else {
			res = chdir(argv[1]);
			if(res == -1)
				perror(argv[1]);
		}
	}
}

int execute(char * const *argv)
{
	int p;
	p = fork();
	if(p == -1)
		perror("fork");
	if(p == 0) {
		execvp(argv[0], argv);
		perror(argv[0]); 
		fflush(stderr);
		_exit(1);
	}
	return p;
}
