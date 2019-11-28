#include "fuzz.h"

int		fuzzy_traversal(t_ast_node *head)
{
	int	i;

	i = 0;
	while (i < MAX_AST_CHILDREN && head->children[i])
	{
		if (head->children[i]->arg)
		{
			if (head->children[i]->arg->type == STR)
				head->children[i]->arg = gen_str(NULL);
			else if (head->children[i]->arg->type == INT)
				head->children[i]->arg = gen_int(NULL);
		}
		i++;
	}
	return (1);
}
