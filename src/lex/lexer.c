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
	arr[';'] = SEMI;
	arr['='] = EQUALS;
	arr[' '] = WHITESPACE;
	arr['\t'] = WHITESPACE;
	arr['\n'] = WHITESPACE;
	arr['\v'] = WHITESPACE;
	arr['\r'] = WHITESPACE;
	arr['\f'] = WHITESPACE;
	return ((e_lexeme)arr[(int)data]);
}

e_lexeme	fuzzy_lexer_classify(char *data)
{
	e_lexeme	ret;

	ret = NONE;
	if (!((ret = fuzzy_lexer_singletons(*data)) == NONE) && *(data + 1) != '=')
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
		if (*(str + i) >= '0' && *(str + i) <= 9)
			return (i);
		if ((*(str + i) >= 'A' && *(str + i) <= 'Z') ||
			(*(str + i) >= 'a' && *(str + i) <= 'z'))
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
	new->dec = 0;
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
	else if (lexeme->type == SEMI)
		printf("SEMI");
	else if (lexeme->type == EQUALS)
		printf("EQUALS");
	else if (lexeme->type == WHITESPACE)
		printf("WHITESPACE");
	else if (lexeme->type == ARG_REF)
		printf("ARG_REF");
	else
		printf("ERR");
}

int				fuzzy_find_eon(char *data)
{
	int	i;

	i = 0;
	while (*(data + i) >= '0' && *(data + i) <= '9')
		i++;
	return (i);
}

t_lexeme		*fuzzy_lexer(char *format)
{
	t_lexeme		*ret;
	e_lexeme		type;
	int				i;
	int				inc;

	ret = NULL;
	type = 0;
	i = 0;
	while (*(format + i))
	{
		inc = 0;
		if ((type = fuzzy_lexer_classify(format + i)) == DATATYPE || type == DOLLAR)
		{
			inc = type == DOLLAR
				? fuzzy_find_eon(format + i + 1) + 1
				: fuzzy_find_eos(format + i + 1) + 1;
			insert_lex(format, i + 1, i + inc, type == DOLLAR ? ARG_REF : type, &ret);
			i += inc - 1;
		}
		else if (type == NONE)
		{
			while (type == NONE && *(format + i + inc))
			{
				inc++;
				type = fuzzy_lexer_classify(format + i + inc);
			}
			insert_lex(format, i, i + inc, VARIABLE, &ret);
			i += (inc - 1);
		}
		else
		{
			if (type == PERCENT || type == EQ || type == GREATEQ || type == LESSEQ)
				inc++;
			insert_lex(format, i, i + 1 + inc, type, &ret);
			i += inc;
		}
		i++;
	}
	return (ret);
}
