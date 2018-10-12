/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 09:26:43 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 19:01:02 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int	write_dbuff(t_list *buffer, char c, int *index)
{
	char 			*tmp;
	static size_t	buff_size = 1024;

	if (*index >= (int)buffer->content_size)
	{
		tmp = ft_memalloc(1 + buffer->content_size + buff_size);
		if (tmp == NULL)
			return (-1);
		buffer->content_size = buffer->content_size + buff_size;
		ft_strcpy(tmp, buffer->content);
		free(buffer->content);
		buffer->content = tmp;
	}
	((char*)buffer->content)[*index] = c;
	*index = *index + 1;
	return (0);
}

int putstr_dbuff(t_list *buffer, char *str, int *index)
{
	while (*str)
	{
		if (write_dbuff(buffer, *str, index) != 0)
			return (-1);
		str++;
	}
	return (0);
}

void	putstr_stderr(char *str)
{
	int len;

	len = ft_strlen(str);
	write(2, str, len);
}

void	free_tab_bytes(char **tab)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		i++;
	}
}

void	free_tab(char **tab)
{
	int i;

	i = 0;
	while (tab[i] != NULL)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}


void	free_cmdlst(t_command *command_lst)
{
	t_command *tmp;

	tmp = command_lst;
	while (command_lst != NULL)
	{
		if (command_lst->args != NULL)
			free_tab(command_lst->args);
		if (command_lst->process_env != NULL)
			free_tab(command_lst->process_env);
		tmp = command_lst->next;
		free(command_lst);
		command_lst = tmp;
	}
	return;
}


