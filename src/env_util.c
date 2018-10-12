/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 10:27:25 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 17:08:15 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_env_char(char c)
{
	if ((ft_isalnum(c)) || (c == '_'))
		return (1);
	return (0);
}

int	valid_env_name(char *str)
{
	size_t i;

	i = 0;
	while (str[i])
	{
		if (!valid_env_char(str[i]))
			return (0);
		i++;
	}
	if (i != ft_strlen(str))
		return (0);
	return (1);
}

char	*construct_env_var(const char *name, const char *value)
{
	size_t		tot_len;
	size_t		len_name;
	char		*new;

	len_name = ft_strlen(name);
	tot_len = len_name + ft_strlen(value) + 1;
	new = ft_strnew(tot_len);
	if (new == NULL)
		return (NULL);
	ft_strcpy(new, name);
	new[len_name] = '=';
	ft_strcpy(new + len_name + 1, value);
	return (new);		
}

void	delete_env_node(t_environ *to_del)
{
	if (to_del->previous != NULL)	
		to_del->previous->next = to_del->next;
	else
		*g_environ = to_del->next;
	if (to_del->next != NULL)
		to_del->next->previous = to_del->previous;
	free(to_del->name);
	if (to_del->value != NULL)
		free(to_del->value);
	free(to_del);
}

int		lst_len(t_environ *const env_lst)
{
	int 		len;
	t_environ	*tmp;

	len = 0;
	tmp = env_lst;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		len++;
	}
	return (len);
}

char	**lst_to_tab(t_environ *const env_lst, int size)
{
	int			len;
	t_environ	*tmp;
	char		**tab;
	
	len = lst_len(env_lst);
	tab = ft_memalloc((len + size + 1) * sizeof (char*));
	if (tab == NULL)
		return (NULL);
	len = 0;
	tmp = env_lst;
	while (tmp != NULL)
	{
		tab[len] = construct_env_var(tmp->name, tmp->value);
		if (tab[len] == NULL)
		{
			free_tab(tab);
			return (NULL);
		}
		len++;
		tmp = tmp->next;
	}
	tab[len] = NULL;
	return (tab);
}
