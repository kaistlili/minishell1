/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 14:53:12 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 19:25:20 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "ft_parser.h"

t_tree	*parse_cmd(char *line, size_t *index)
{
	size_t start;
	char *cmd_line;
	t_tree	*new;

	start = *index;
	while (line[*index])
	{
		if ((line[*index] == ';') || (line[*index] == '|'))
			break;
		*index = *index + 1;
	}
	if ((cmd_line = ft_strnew(*index - start)) == NULL)
		return (NULL);
	ft_strncpy(cmd_line, line + start, *index - start);
	if (!(new = tr_new_node(CMD)))
	{
		free(cmd_line);
		return (NULL);
	}
	new->type = cmd;
	new->data = new_cmd_node(cmd_line);
	free(cmd_line);
//	if (new->data == NULL) we get confused with malloc errors here
//		return (NULL);
	return (new);
}

int		add_t_node(t_tree *node, t_tree **head)
{
	int ret;

	ret = 0;
	if (*head == NULL)
		*head = node;
	else
	{
//		if (((node->type == sep) && ((*head)->type != cmd))
//			&& (((*head)->left == NULL) || ((*head)->right == NULL)))
//			return (SYNERR);
		
		if (node->type == cmd)
			ret = add_cmd(node, head);
		else if (node->type == e_pipe)
			ret = add_pipe(node, head);
		else if (node->type == sep)
			ret = add_sep(node, head);
	}
	return (ret);

}

int	ft_build_tree(char *line, t_tree **tree)
{
	size_t	i;
	int		ret;
	t_tree	*node;
//	char	**tokens;

//	tokens = ft_splitblanks(line);
//	if (tokens == NULL)
//		return (0);
	i = 0;
	ret = 0;
	while (line[i])
	{
		while ((ft_isblank(line[i]) && (line[i])))
			i++;
		if (!line[i])
			break;
		if (!ft_strncmp(";", line + i, 1))
			node = tr_new_node(SEP);
		else if (!ft_strncmp("|", line + i, 1))
			node = tr_new_node(PIPE);
		else
			node = parse_cmd(line, &i);
		if (node == NULL)
			return (MEMERR);
		if (node->data == NULL)
			i = i + 1;
		ret = add_t_node(node, tree); //aggregate nodes logic
		if (ret == SYNERR)
			break;
	}
	return (ret);
}
