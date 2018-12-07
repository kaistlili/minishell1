/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 10:27:25 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 17:08:15 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_parser.h"

t_tree	*tr_new_node(int type)
{
	t_tree *new;

	if (!(new = ft_memalloc(sizeof(t_tree))))
		return (NULL);
	new->type = type;
	return (new);	
}

int	add_cmd(t_tree *node, t_tree **head)
{
	t_tree *insert_pos;

	insert_pos = *head;
	while (insert_pos->right != NULL)
		insert_pos = insert_pos->right;
	if ((*head)->type == cmd) //redundant ?
		return(SYNERR);
	insert_pos->right = node;
	return (0);
}

int	add_pipe(t_tree *node, t_tree **head)
{
	if ((*head)->type == sep)
	{
		if ((*head)->right == NULL)
			return (SYNERR);
		node->left = (*head)->right;
		(*head)->right = node;
	}
	else
	{
		node->left = *head;
		*head = node;
	}
	return (0);
}

int	add_sep(t_tree *node, t_tree **head)
{
	node->left = *head;
	*head = node;
	return (0);
}

void	free_t_node(t_tree *node)
{
	if (node->data != NULL)
	{
		if (node->data->args != NULL)
			free_tab(node->data->args);
		if (node->data->process_env)
			free_tab(node->data->process_env);
		free(node->data);
	}
	free(node);
}

void free_tree(t_tree *head)
{
	if (head == NULL)
		return;
	free_tree(head->left);
	free_tree(head->right);
	free_t_node(head);
	return;	

}
