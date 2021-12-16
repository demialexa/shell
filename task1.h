#ifndef TASK1_H_SENTRY
#define TASK1_H_SENTRY

#define BGROUND (char*)-1
#define REDIR_IN (char*)-2
#define REDIR_OUT (char*)-3
#define REDIR_APPEND (char*)-4
#define PIPE (char*)-5

enum { redir_append = -2 };

struct item {
	char *word;
	struct item *next;
};

int iftag(char *word);

void print_line(const struct item *first);

void free_line(struct item *first);

int get_line(struct item **first);

#endif
