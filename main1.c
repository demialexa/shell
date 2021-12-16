#include "task1.h"
#include <stdio.h>

int main()
{
	int excode;
	struct item *first;
	do {
		excode = get_line(&first);
		print_line(first);
		free_line(first);
	} while(excode != EOF);
	return 0;
}
