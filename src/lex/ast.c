#include "fuzz.h"

int			is_arg_no(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (atoi(lex->data) && lex->type == ARG_REF)
	{
		if (lex->dec != ARG_NO)
			lex->dec = ARG_NO;
		return (1);
	}
	return (0);
}

int			is_cmp_op(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (lex->type == GREAT || lex->type == LESS
		|| lex->type == GREATEQ || lex->type == LESSEQ
		|| lex->type == MOD || lex->type == EQ)
	{
		if (lex->dec != CMP_OP)
			lex->dec = CMP_OP;
		return (1);
	}
	return (0);
}

int			is_string(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (lex->data[0] && lex->type == VARIABLE)
	{
		if (lex->dec != STRING)
			lex->dec = STRING;
		return (1);
	}
	return (0);
}

int			is_number(t_lexeme *lex)
{
	int	n;

	if (!lex)
		return (0);
	n = atoi(lex->data);
	if ((n || (!n && lex->data[0] == '0')) && lex->type == VARIABLE)
	{
		if (lex->dec != NUMBER)
			lex->dec = NUMBER;
		return (1);
	}
	return (0);
}

int			is_var(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (is_number(lex) || is_string(lex))
	{
		if (lex->dec != VAR)
			lex->dec = VAR;
		return (1);
	}
	return (0);
}

int			is_cmp(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if ((is_cmp_op(lex) && is_var(lex->next))
		|| (is_cmp_op(lex) && is_arg_no(lex->next)))
	{
		if (lex->dec != CMP)
			lex->dec = CMP;
		return (1);
	}
	return (0);
}

int			is_not(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (lex->type == EXCL && lex->next)
	{
		if (lex->dec != NOT)
			lex->dec = NOT;
		return (1);
	}
	return (0);
}

int			is_constr(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (is_cmp(lex) || (is_not(lex) && is_cmp(lex->next)))
	{
		if (lex->dec != CONSTR)
			lex->dec = CONSTR;
		return (1);
	}
	return (0);
}

int			is_type(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (lex->type == DATATYPE)
	{
		if(lex->dec != TYPE)
			lex->dec = TYPE;
		return (1);
	}
	return (0);
}

int			is_eq(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (lex->type == EQUALS && lex->next)
	{
		if (lex->dec != EQUALITY)
			lex->dec = EQUALITY;
		return (1);
	}
	return (0);
}

int			is_arg(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (is_type(lex) || (is_type(lex) && is_constr(lex->next)) ||
		(is_type(lex) && is_eq(lex->next)))
	{
		if (lex->dec != ARG)
			lex->dec = ARG;
		return (1);
	}
	return (0);
}

int			is_expr(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if ((is_arg(lex) && is_arg(lex->next)) || is_arg(lex))
	{
		if (lex->dec != EXPR)
			lex->dec = EXPR;
		return (1);
	}
	return (0);
}

int			is_block_start(t_lexeme *lex)
{
	t_lexeme	*tmp;

	if (!(tmp = lex))
		return (0);
	if ((tmp->type == OPENCURLY || tmp->type == OPENPAR) && tmp->next)
	{
		while (tmp && tmp->next)
		{
			if (tmp->type == CLOSECURLY || tmp->type == CLOSEPAR)
			{
				if (tmp->dec != BLOCK_END)
					tmp->dec = BLOCK_END;
				if (lex->dec != BLOCK_START)
					lex->dec = BLOCK_START;
				return (1);
			}
			tmp = tmp->next;
		}
	}
	return (0);
}

int			is_sep(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (lex->type == SEMI && lex->next)
	{
		if (lex->dec != SEP)
			lex->dec = SEP;
		return (1);
	}
	return (0);
}

void		fuzzy_parser_print_enum(e_ast_node e)
{
	if (e == EXPR)
		printf("EXPR");
	else if (e == ARG)
		printf("ARG");
	else if (e == TYPE)
		printf("TYPE");
	else if (e == CONSTR)
		printf("CONSTR");
	else if (e == NOT)
		printf("NOT");
	else if (e == CMP)
		printf("CMP");
	else if (e == CMP_OP)
		printf("CMP_OP");
	else if (e == ARG_NO)
		printf("ARG_NO");
	else if (e == VAR)
		printf("VAR");
	else if (e == BLOCK_START)
		printf("BLOCK_START");
	else if (e == BLOCK_END)
		printf("BLOCK_END");
	else if (e == NUMBER)
		printf("NUMBER");
	else if (e == STRING)
		printf("STRING");
	else if (e == EQUALITY)
		printf("EQUALITY");
	else if (e == SEP)
		printf("SEP");
	else if (e == EMPTY)
		printf("EMPTY");
}

e_ast_node	fuzzy_parser_classify(t_lexeme *lex)
{
	if (!lex)
		return (NONE);
	if (lex->dec)
		return (lex->dec);
	//if (is_expr(lex))
		//return (EXPR);
	if (is_arg(lex))
		return (ARG);
	if (is_type(lex))
		return (TYPE);
	if (is_constr(lex))
		return (CONSTR);
	if (is_not(lex))
		return (NOT);
	if (is_var(lex))
		return (VAR);
	if (is_cmp_op(lex))
		return (CMP_OP);
	if (is_arg_no(lex))
		return (ARG_NO);
	if (is_eq(lex))
		return (EQUALITY);
	if (is_block_start(lex))
		return (BLOCK_START);
	if (is_sep(lex))
		return (SEP);
	if (is_number(lex))
		return (NUMBER);
	if (is_string(lex))
		return (STRING);
	return (EMPTY);
}

t_ast_node	*fuzzy_parser(t_lexeme *lex)
{
	t_lexeme	*tmp;

	tmp = lex;
	while (tmp)
	{
		printf("LEX: %.*s | CLASS: ", tmp->len, tmp->data);
		fuzzy_parser_print_enum(fuzzy_parser_classify(tmp));
		printf("\n");
		tmp = tmp->next;
	}
	return (NULL);
}
