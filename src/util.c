/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/07 16:10:23 by ktlili            #+#    #+#             */
/*   Updated: 2018/09/24 12:24:38 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


int	ft_ispath(char *str)
{
	while (*str)
	{
		if (*str == '/')
			return (1);
		str++;
	}
	return (0);
}

int	add_slash(char **path)
{
	int		len;
	char	*tmp;

	len = ft_strlen(*path);
	if ((len != 0) && ((*path)[len - 1] != '/'))
	{
		tmp = ft_strjoin(*path, "/");
		if (tmp == NULL)
			return (MEMERR);
		free(*path);
		*path = tmp;		
	}
	return (0);
}

int	path_access(char *path)
{
	struct stat target_data;
	
	if (stat(path, &target_data) != 0)
		return (2);
	if (!S_ISDIR(target_data.st_mode))
		return (5);
	if (access(path, X_OK) != 0)
		return (3);
	return (0);
}

int	dir_access(char *cd_path, char **curpath)
{
	struct stat target_data;
	char 		*tmp_path;

	tmp_path = ft_strjoin(cd_path, *curpath);
	if (tmp_path == NULL)
		return (MEMERR);
	if (stat(tmp_path, &target_data) != 0)
	{
		free(tmp_path);
		return (-1); //???? to change ??
	}
	if ((S_ISDIR(target_data.st_mode)) && (!access(tmp_path, X_OK)))
	{
		free(*curpath);
		*curpath = tmp_path;
		return (0);
	}
	free(tmp_path);
	return (-1);
}

int	ft_cmptab(char **tab, char *str)
{
	int	i;

	i = 0;
	while (tab[i] != NULL)
	{
		if (!ft_strcmp(tab[i], str))
			return (i);
		i++;
	}
	return (-1);
}

void		add_cmdlst(t_command **head, t_command *to_add)
{
	t_command *tmp;

	if (*head == NULL)
	{
		*head = to_add;
		return;
	}
	tmp = *head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = to_add;
	to_add->previous = tmp;
}
/*
t_command	*new_cmd_node(char **space_split)
{
	t_command	*new;

	new = malloc(sizeof(t_command));
	if (new == NULL)
		return (NULL);
	ft_bzero(new, sizeof(t_command));
	if (space_split != NULL)
		new->args = dup_tab(space_split);
	return (new);
}
*/

size_t	tablen(char **split)
{
	size_t len;

	len = 0;
	while (split[len] != NULL)
		len++;
	return (len);
}

t_command	*new_cmd_node(char *cmd_line)
{
	t_command	*new;
	char		**split;

	split = ft_splitblanks(cmd_line);
	new = ft_memalloc(sizeof(t_command));
	if ((new == NULL) || (split == NULL))
		return (NULL);
	new->args = split;
	new->agcount = tablen(split);
	return (new);
}

char	**dup_tab(char **tab)
{
	size_t	len;
	size_t	i;
	char 	**copy;

	len = 0;
	i = 0;
	while (tab[len] != NULL)
		len++;
	copy = malloc((len + 1) * sizeof(char*));
	if (copy == NULL)
		return (NULL);
	while (i < len)
	{
		copy[i] = ft_strdup(tab[i]);
		if (copy[i] == NULL)
		{
			free_tab(copy);
			return (NULL);		
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	**dup_ntab(char **tab, size_t n)
{
	size_t	i;
	char 	**copy;

	i = 0;
	copy = malloc((n + 1) * sizeof(char*));
	if (copy == NULL)
		return (NULL);
	while (i < n)
	{
		copy[i] = ft_strdup(tab[i]);
		if (copy[i] == NULL)
		{
			free_tab(copy);
			return (NULL);		
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
