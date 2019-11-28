#include "fuzz.h"

void	fuzzy_substitute_string(t_ast_node *node, va_list list)
{
	char	*data = va_arg(list, char*);
	node->lexeme->data = data;
	node->lexeme->len = strlen(data);
}

int		fuzzy_substitutions(t_ast_node *head, va_list list)
{
	int	i;

	if (!head)
		return (0);
	if (head->lexeme)
		printf("subbing %s\n", head->lexeme->data);
	i = 0;
	if (head->type == ARG && !strncmp(head->lexeme->data, "s", head->lexeme->len))
		fuzzy_substitute_string(head, list);
	while (i < MAX_AST_CHILDREN && head->children[i])
		fuzzy_substitutions(head->children[i++], list);
	return (1);
}
