#include "fuzz.h"

int		fuzzy_traversal(t_ast_node *head)
{
	int	i;

	i = 0;
	while (i < MAX_AST_CHILDREN && head->children[i])
	{
		
		i++;
	}
}

int		replace_subs(t_ast_node *head)
{
	int		i;
	t_lexeme	*tmp;
	t_ast_node	*start;
	int		idx;

	i = 0;
	if (!(start = head))
		return (0);
	if (head->type == ARG_NO)
	{
		tmp = head->lexeme;
		start = head;
		while (start->parent)
			start = start->parent;
		idx = atoi(head->lexeme->data);
		if (idx >= MAX_AST_CHILDREN || !start->children[idx])
			printf("-fuzzy: %s: invalid argument number\n", head->lexeme->data);
		else
		{
			tmp = head->lexeme;
			head->lexeme = start->children[idx]->lexeme;
			free(tmp);
			head->type = VAR
		}
	}
	while (i < MAX_AST_CHILDREN && head->children[i])
	{
		if (head->children[i]->type == ARG_NO)
			replace_subs(head->children[i]);
		i++;
	}
	return (1);
}

int		fuzzy_traverse_ast(t_ast_node *head)
{
	
}
