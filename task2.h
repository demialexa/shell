#ifndef	TASK2_H_SENTRY
#define TASK2_H_SENTRY

void free_argv(char ***argv);

int vec_equal(const char *vec1, const char *vec2);

char **to_argv(const struct item *first);

void ch_dir(char * const *argv);

int execute(char * const *argv);

#endif
