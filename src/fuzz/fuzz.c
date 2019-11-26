#include "fuzz.h"

void	*fuzz(void (*f)(void), const char *fmt, ...)
{
	if (!f || !fmt)
		return (NULL);
	//...
	return (NULL);
}

int		main(int argc, char **argv)
{
	t_lexeme	*tmp;
	t_lexeme	*x;

	if (argc != 2)
		return(printf("not enough args\n"));
	tmp = fuzzy_lexer(argv[1]);
	x = tmp;
	while (tmp)
	{
		printf("[%.*s](", tmp->len, tmp->data);
		print_enum_type(tmp);
		printf(")\n");
		tmp = tmp->next;
	}
	fuzzy_parser(x);
}
