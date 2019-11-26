#include "fuzz.h"

e_lexeme	fuzzy_lexer_singletons(char data)
{
	static int	arr[127];

	arr['%'] = DATATYPE;
	arr['{'] = OPENCURLY;
	arr['}'] = CLOSECURLY;
	arr['('] = OPENPAR;
	arr[')'] = CLOSEPAR;
	arr['+'] = PLUS;
	arr['-'] = MINUS;
	arr['*'] = MULT;
	arr['/'] = DIV;
	arr['$'] = DOLLAR;
	arr['>'] = GREAT;
	arr['<'] = LESS;
	arr['!'] = EXCL;
	arr['\"'] = QUOTE;
	arr['\''] = LITQUOTE;
	return ((e_lexeme)arr[(int)data]);
}

e_lexeme	fuzzy_lexer_classify(char *data)
{
	e_lexeme	ret;

	ret = NONE;
	if (!((ret = fuzzy_lexer_singletons(*data)) == NONE))
		return (ret);
	if (!strncmp(data, "%%", 2))
		return (PERCENT);
	if (!strncmp(data, "==", 2))
		return (EQ);
	if (!strncmp(data, ">=", 2))
		return (GREATEQ);
	if (!strncmp(data, "<=", 2))
		return (LESSEQ);
	return (ret);
}

static int		fuzzy_find_eos(char *str)
{
	int	i;

	i = 0;
	while (*(str + i))
	{
		if ((*(str + i) >= 'A' && *(str + i) <= 'Z') ||
			(*(str + i) >= 'a' && *(str +i ) <= 'z'))
			i++;
		else
			return (i);
	}
	return (i);
}

t_lexeme		*new_lex(char *data, int start, int end, e_lexeme type)
{
	t_lexeme	*new;

	new = malloc(sizeof(*new));
	new->next = NULL;
	new->data = data + start;
	new->len = end - start;
	new->type = type;
	return (new);
}

void			insert_lex(char *data, int start, int end, e_lexeme type, t_lexeme **list)
{
	t_lexeme	*tmp;
	t_lexeme	*new;

	tmp = *list;
	new = new_lex(data, start, end, type);
	if (!tmp)
		*list = new;
	else
	{
		while (tmp && tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void			print_enum_type(t_lexeme *lexeme)
{
	if (!lexeme)
		return ;
	if (lexeme->type == NONE)
		printf("NONE");
	else if (lexeme->type == PERCENT)
		printf("PERCENT");
	else if (lexeme->type == DATATYPE)
		printf("DATATYPE");
	else if (lexeme->type == OPENCURLY)
		printf("OPENCURLY");
	else if (lexeme->type == CLOSECURLY)
		printf("CLOSECURLY");
	else if (lexeme->type == OPENPAR)
		printf("OPENPAR");
	else if (lexeme->type == CLOSEPAR)
		printf("CLOSEPAR");
	else if (lexeme->type == PLUS)
		printf("PLUS");
	else if (lexeme->type == MINUS)
		printf("MINUS");
	else if (lexeme->type == MULT)
		printf("MULT");
	else if (lexeme->type == DIV)
		printf("DIV");
	else if (lexeme->type == EQ)
		printf("EQ");
	else if (lexeme->type == DOLLAR)
		printf("DOLLAR");
	else if (lexeme->type == VARIABLE)
		printf("VARIABLE");
	else if (lexeme->type == EXCL)
		printf("EXCL");
	else if (lexeme->type == GREAT)
		printf("GREAT");
	else if (lexeme->type == LESS)
		printf("LESS");
	else if (lexeme->type == GREATEQ)
		printf("GREATEQ");
	else if (lexeme->type == LESSEQ)
		printf("LESSEQ");
	else if (lexeme->type == QUOTE)
		printf("QUOTE");
	else if (lexeme->type == LITQUOTE)
		printf("LITQUOTE");
	else
		printf("ERR");
}

t_lexeme		*fuzzy_lexer(char *format)
{
	t_lexeme		*ret;
	e_lexeme	type;
	int				i;
	int				inc;

	ret = NULL;
	type = 0;
	i = 0;
	while (*(format + i))
	{
		inc = 0;
		if ((type = fuzzy_lexer_classify(format + i)) == DATATYPE)
		{
			inc = fuzzy_find_eos(format + i + 1) + 1;
			insert_lex(format, i + 1, inc, type, &ret);
			i += inc - 1;
		}
		else
			insert_lex(format, i, i + 1, type, &ret);
		i++;
	}
	return (ret);
}
