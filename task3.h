#ifndef	TASK3_H_SENTRY
#define TASK3_H_SENTRY

enum {
	bg = 0,
	fg = 1,
	bg_err = -1
};

void zombie_clean();

void chld_hdl(int signo);

int pop_bgtag(char ***argv);

#endif
