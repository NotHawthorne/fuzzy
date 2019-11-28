#include "fuzz.h"

void	*fuzz(void (*f)(void), const char *fmt, ...)
{
	t_lexeme	*lexemes;
	t_ast_node	*ast;
	va_list		vargs;

	if (!f || !fmt)
		return (NULL);
	va_start(vargs, fmt);
	lexemes = fuzzy_lexer((char*)fmt);
	ast = fuzzy_parser(lexemes, vargs);
	//fuzzy_substitutions(ast, vargs);
	va_end(vargs);
	//printf("AFTER SUB\n");
	fuzzy_ast_print(ast);
	return (NULL);
}

int		main(int argc, char **argv)
{
	if (argc == 1)
		printf("not enough args\n");
	fuzz((void (*)(void))printf, argv[1], argv[2], argv[3]);
}
