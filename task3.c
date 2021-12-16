#include "task1.h"
#include "task3.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int pop_bgtag(char ***argv)
{
	int i;
	if(!*argv)
		return fg;
	for(i = 0; (*argv)[i]; i++) {
		if((*argv)[i] == BGROUND) {
			if((*argv)[i+1]) {
				fputs("&: Incorrect command\n", stderr);
				return bg_err;
			} else {
				if(i) {
					(*argv)[i]  = NULL;
				} else {
					*argv = NULL;
				}
				return bg;
			}
		}
	}
	return fg;
}

void zombie_clean()
{
	int w;
	while((w = waitpid(-1, NULL, WNOHANG)) && w != -1)
		;
}
	
void chld_hdl(int signo)
{
	int w;
//	const char msg[] = "zombie disappears\n";
	signal(SIGCHLD, chld_hdl);
	while((w = waitpid(-1, NULL, WNOHANG)) && w != -1)
		;
//	write(1, msg, sizeof(msg));
}
