#include "minishell.h"
#include "ft_parser.h"


int	assert_sep(t_tree *node, int lvl)
{
	if (lvl == 0)
	{
		if ((node->left == NULL)
			|| ((node->right == NULL)
				&& (node->left->type == SEP)))
			return (SYNERR);
		return (0);
	}
	if ((node->left == NULL) || (node->right == NULL))
		return (SYNERR);
	return (0);
}

int	assert_pipe(t_tree *node, int lvl)
{
	if (lvl == 0)
	{
		if (node->left == NULL)
			return (SYNERR);
		return(0); 
	}
	if ((node->left == NULL) || (node->right == NULL))
		return (SYNERR);
	return (0);
}

int	assert_syntax(t_tree *head, int level)
{
	static int lvl = 0;

	if (!head)
		return (SYNERR);
	lvl = level;
	if (head->type == SEP)
	{
		if (assert_sep(head, lvl))
			return (SYNERR);
	}
	else if (head->type == PIPE)
	{
		if (assert_pipe(head, lvl))
			return (SYNERR);
	}
	else
		return (0);
	lvl++;
	if ((assert_syntax(head->left, lvl))
		||	(assert_syntax(head->right, lvl)))	
		return (SYNERR);
	return (0);
}
