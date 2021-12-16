#include "task1.h"
#include <stdio.h>
#include <stdlib.h>

int iftag(char *word)
{
	int i, sz;
	char *sep_tag[] = { BGROUND, REDIR_IN, REDIR_OUT, REDIR_APPEND, PIPE };
	sz = sizeof(sep_tag)/sizeof(*sep_tag);
	for(i = 0; i < sz; i++) {
		if(word == sep_tag[i]) {
			return 1;
		}
	}
	return 0;
}

void print_line(const struct item *first)
{
	int i;
	if(first) {
		print_line(first->next);
		if((i = iftag(first->word)))
			printf("***special***\n");
		else {
			puts(first->word);
		}
	}
}

void free_line(struct item *first)
{
	struct item *tmp;
	while(first) {
		tmp = first;
		first = first->next;
		if(!iftag(tmp->word))
			free(tmp->word);
		free(tmp);
	}
}

static char *add_spot(char **word, int size)
{
	int i;
	char *new_word;
	new_word = malloc((size+2) * sizeof(*new_word));
	for(i = 0; i < size; i++)
		new_word[i] = (*word)[i];
	new_word[i+1] = '\0';
	free(*word);
	return new_word;
}

static int ifappend()
{
	int c;
	c = getchar();
	if(c == '>')
		return 1;
	ungetc(c, stdin);
	return 0;
}

static int sep_code(char c)
{
	int i, sepcode = 0;
	char sep[] = { ' ', '\n', '&', '>', '<', '|' };
	for(i = 0; i < sizeof(sep); i++) {
		if(c == sep[i]) {
			sepcode = c;
			break;
		}
	}
	if(c == '>' && ifappend())
		sepcode = redir_append; /* >> - append output */
	return sepcode;
}

static int get_word(char **word)
{
	int excode, c, quotes = 0, bslash = 0, size = 0;
	*word = NULL;
	while((c = getchar()) != EOF) {
		if(!bslash && (c == '\\' || c == '"')) {
			c == '"'? (quotes = !quotes): (bslash = 1);
			continue;
		}
		if(!quotes && (excode = sep_code(c))) {
			if(!size && excode == ' ')
				continue;
			return excode;
		}
		*word = add_spot(word, size);
		(*word)[size] = c;
		size++;
		bslash = 0;
	}
	free(*word);
	*word = NULL;
	return EOF;
}

static struct item *add_node(struct item *first, char* word)
{
	struct item *new_first;
	new_first = malloc(sizeof(*new_first));
	new_first->word = word;
	new_first->next = first;
	return new_first;
}

static char* get_tag(int code)
{
	int i;
	char* sep_tag[] = { BGROUND, REDIR_IN, REDIR_OUT, REDIR_APPEND, PIPE };
	int sep_code[] = { '&', '<', '>', redir_append, '|' };
	for(i = 0; i < sizeof(sep_code); i++) {
		if(code == sep_code[i])
			return sep_tag[i];
	}
	return NULL;
}

int get_line(struct item **first)
{
	int excode;
	char *buf, *tag;
	*first = NULL;
	while((excode = get_word(&buf)) != EOF) {
		if(buf) {	
			*first = add_node(*first, buf);
		}
		if(excode == ' ')
			continue;
		if(excode == '\n')
			return '\n';
		tag = get_tag(excode);
		*first = add_node(*first, tag); 
	}
	free_line(*first);
	*first = NULL;
	return EOF;
}
