#include "fuzz.h"

t_ast_node	*fuzzy_ast_new_node(t_lexeme *lexeme, e_ast_node type, t_ast_node *parent)
{
	t_ast_node	*new;
	int			i;

	i = 0;
	new = malloc(sizeof(*new));
	new->lexeme = lexeme;
	new->type = type;
	new->parent = parent;
	new->arg = 0;
	memset(&(new->children), 0, MAX_AST_CHILDREN * sizeof(struct s_ast_node*));
	while (parent && i < MAX_AST_CHILDREN && parent->children[i])
		i++;
	if (parent && i < MAX_AST_CHILDREN)
		parent->children[i] = new;
	return (new);
}

void	fuzzy_ast_print(t_ast_node *head)
{
	static int	depth;
	int			i;
	char		c[] = "--------------------------------";

	if (!head)
		return ;
	depth++;
	i = 0;
	if (head->arg)
	{
		if (head->arg->type == STR)
			printf("%.*s%.*s\n", depth * 2, c, (int)strlen(head->arg->data.s), head->arg->data.s);
		else if (head->arg->type == INT)
			printf("%.*s%d\n", depth * 2, c, head->arg->data.d);
	}
	else if (head->lexeme && head->lexeme->data)
		printf("%.*s%.*s\n", depth * 2, c, head->lexeme->len, head->lexeme->data);
	while (i < MAX_AST_CHILDREN && head->children[i])
		fuzzy_ast_print(head->children[i++]);
	depth--;
}

t_arg	*construct_arg(e_datatype type, va_list list)
{
	t_arg	*new;

	new = malloc(sizeof(*new));
	if (type == STR)
		new->data.s = va_arg(list, char*);
	else if (type == INT || type == CHAR)
		new->data.d = va_arg(list, int);
	else if (type == INT_ARR)
		new->data.dp = va_arg(list, int*);
	else
	{
		free(new);
		return (NULL);
	}
	new->next = NULL;
	new->type = type;
	return (new);
}

e_datatype	fuzzy_interp_datatype(t_lexeme *lex)
{
	if (!lex || !lex->data)
		return (0);
	if (!strncmp(lex->data, "s", lex->len))
		return (STR);
	if (!strncmp(lex->data, "d", lex->len))
		return (INT);
	if (!strncmp(lex->data, "dp", lex->len))
		return (INT_ARR);
	if (!strncmp(lex->data, "c", lex->len))
		return (CHAR);
	return (ERROR);
}

void	fuzzy_ast_insert(t_ast_node **head, e_ast_node type, t_lexeme *lex, va_list list)
{
	t_ast_node	*new;
	t_ast_node	*tmp;

	if (!*head)
		*head = fuzzy_ast_new_node(NULL, EXPR, NULL);
	tmp = *head;
	if (type == ARG)
	{
		while (tmp->parent)
			tmp = tmp->parent;
		new = fuzzy_ast_new_node(lex, type, tmp);
		new->arg = construct_arg(fuzzy_interp_datatype(lex), list);
		*head = new;
	}
	else if (type == VAR || type == ARG_NO)
	{
		new = fuzzy_ast_new_node(lex, type, *head);
		if (lex->next && lex->next->dec != MATH_OP)
		{
			while (tmp->parent && tmp->type != EQUALITY)
				tmp = tmp->parent;
			*head = tmp;
		}
	}
	else if (type == MATH_OP)
		new = fuzzy_ast_new_node(lex, type, *head);
	else
	{
		new = fuzzy_ast_new_node(lex, type, *head);
		*head = new;
	}
	if (!lex->next)
		while ((*head)->parent)
			*head = (*head)->parent;
}
