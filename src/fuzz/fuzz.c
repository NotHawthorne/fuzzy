#include "fuzz.h"

typedef int (*thing_func)(char*, char*, int);

void	*fuzz(void (*f)(void), const char *fmt, ...)
{
	t_lexeme	*lexemes;
	t_ast_node	*ast;
	va_list		vargs;
	thing_func	test;

	if (!f || !fmt)
		return (NULL);
	va_start(vargs, fmt);
	lexemes = fuzzy_lexer((char*)fmt);
	ast = fuzzy_parser(lexemes, vargs);
	//fuzzy_substitutions(ast, vargs);
	va_end(vargs);
	//printf("AFTER SUB\n");
	fuzzy_traversal(ast);
	fuzzy_ast_print(ast);
	test = (thing_func)f;
	test("[%s][%d]\n", ast->children[0]->arg->data.s, ast->children[1]->arg->data.d);
	return (NULL);
}

int		main(void)
{
	char	*data;
	int		data2;

	data = malloc(sizeof(char) * 6);
	data[0] = 'H';
	data[1] = 'e';
	data[2] = 'l';
	data[3] = 'l';
	data[4] = 'o';
	data[5] = 0;
	data2 = 10554;
	fuzz((void (*)(void))printf, "%s%d={>5000}%s", "[%d][%s]\n", data2, data);
}
