/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 21:02:31 by ktlili            #+#    #+#             */
/*   Updated: 2018/09/14 02:19:47 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	print_cmd(t_command command)
{
//	ft_printf("command:\n%s \n", command.command);
//	ft_printf("args:\n");
	print_tab(command.args);
//	ft_printf("env\n");
//	print_tab(command.process_env);
}


void	print_env_lst(t_environ *lst)
{
	while (lst != NULL)
	{
		ft_printf("%s=%s\n", lst->name, lst->value);
		lst = lst->next;
	}
}

void	print_cmd_lst(t_command *command)
{
	t_command *tmp;
	int i;

	i = 0;
	ft_printf("++++++++++++++++++++++++++\n");
	tmp = command;
	while (tmp != NULL)
	{
		i++;
		print_cmd(*tmp);
		ft_printf("");
		tmp = tmp->next;
		ft_printf("------------------------\n");
	}
	ft_printf("++++++++++++++++++++++++++\n");
	ft_printf("%d nodes\n", i);
}

void	print_tab(char **tab)
{
	int i;

	i = 0;
	if (tab == NULL)
		return;
	while (tab[i] != NULL)
	{
		ft_printf("%s\n", tab[i]);
		i++;
	}
}
