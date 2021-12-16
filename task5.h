#ifndef TASK5_H_SENTRY
#define TASK5_H_SENTRY

char ***divide_argv(char **argv);

int* pipe_execute(char ***vec_argv, int fd_in, int fd_out);

#endif
