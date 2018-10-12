/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 14:53:12 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 19:25:20 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int parse_multicmd(char *line)
{
	int i;
	int j;

	i = 0;
	j = 0;
	if (line[i] == ';')
		return (SYNERR);
	while (ft_isblank(line[j]))
		j++;
	if (line[j] == ';')
		return (SYNERR);
	while (line[i] != 0)
	{
		if (line[i] == ';')
		{
			j = i + 1;
			while (ft_isblank(line[j]))
				j++;
			if (line[j] == ';')
				return (SYNERR);
		}
		i++;
	}
	return (0);
}

int	construct_cmd(char *line, t_command **curr_cmd)
{
	char	**space_split;

	space_split = ft_splitblanks(line);
	if (space_split == NULL)
		return (MEMERR);		
	*curr_cmd = new_cmd_node(space_split);
	if (*curr_cmd == NULL)
		return (MEMERR); 
	free_tab(space_split);
	return (0);
}

int	parser(char *line, t_command **cmd_lst)
{
	char 		**cmd_split;
	t_command	*curr_cmd;
	int			ret;
	int			i;

	i = 0;
	if (parse_multicmd(line) != 0)
		return (SYNERR);
	cmd_split = ft_strsplit(line, ';');
	if (cmd_split == NULL)
		return (MEMERR);
	while (cmd_split[i] != NULL)	
	{
		ret = construct_cmd(cmd_split[i], &curr_cmd); 
		
		if (ret == 0)
			add_cmdlst(cmd_lst, curr_cmd);
		else
			return (ret);
		i++;
	}
	free_tab(cmd_split);
	return(0);
}
