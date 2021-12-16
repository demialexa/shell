#ifndef	TASK4_H_SENTRY
#define TASK4_H_SENTRY

enum {
	r_in = 1,
	r_out = 2,
	r_append = 3,	
	r_no = -1,
	r_err = -2
};

int get_argc(char **argv);

int pop_redir_out(char ***argv);

int pop_redir_in(char ***argv);

int pop_bgtag(char ***argv);

void redir(int fd1, int fd2);

int redir_execute(char * const *argv, int fd_in, int fd_out, int fg_bg);

#endif
