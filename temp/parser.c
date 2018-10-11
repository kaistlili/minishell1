/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 14:53:12 by ktlili            #+#    #+#             */
/*   Updated: 2018/09/09 22:37:51 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


char	*expand_tokens(char	*line)
{
	int	i;

	i = 0;
	while (line[i] != 0)
	{
		
	}
}

int	parse_line(char *line, t_command **cmd_lst)
{
	char 		**cmd_split;
	t_command	*curr_cmd;
	int			i;

	cmd_split = ft_strsplit(line, ';');
	if (cmd_split == NULL)
		return (MEMERR);
	print_tab(cmd_split);
	while (cmd_split[i] != NULL)	
	{
		cmd_split = expand_tokens(cmd_split[i]); /* replace ~ and $ */
		if (cmd_split == NULL)
			return (MEMERR);
		curr_cmd = construct_cmd(cmd_split[i]); /*split and parse then malloc t_command*/
		if (curr_cmd == NULL)
			return (MEMERR);
		add_cmdlst(cmd_lst, curr_cmd);
		i++;
	}
	return(0);
}
