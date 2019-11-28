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
