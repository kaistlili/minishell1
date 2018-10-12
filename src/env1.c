/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 22:37:38 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 19:35:45 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_value(char *env_var)
{
	int i;

	i = 0;
	while (valid_env_char(env_var[i]))
		i++;
	if (env_var[i] == 0)
		return (env_var + i);
	return (env_var + i + 1);
}

char	*tab_get_value(char *name, char **env)
{
	int i;
	int len;

	if ((name == NULL) || (env == NULL))
		return (NULL);
	i = 0;
	len = ft_strlen(name);
	while (env[i] != NULL)
	{
		if ((!ft_strncmp(name, env[i], len)) && (env[i][len] == '='))
			return (get_value(env[i]));
		i++;
	}
	return (NULL);
}

void	add_node(t_environ **head, t_environ *to_add)
{
	t_environ *tmp;

	if (*head == NULL)
		*head = to_add;
	else
	{
		tmp = *head;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = to_add;
		to_add->previous = tmp;
	}	
}

t_environ *new_env_node(char *name, char *value)
{
	t_environ *new;

	if ((name == NULL) || (value == NULL))
		return (NULL);
	new = ft_memalloc(sizeof(t_environ));
	if (new == NULL)
		return (NULL);
	new->name = name;
	new->value = value;
	return (new);
}

t_environ *env_to_lst(char *env_var)
{
	char *name;
	char *value;
	int	i;

	i = 0;
	while ((env_var[i]) && (env_var[i] != '='))
		i++;
	name = ft_strnew(i);
	if (name == NULL)
		return (NULL);
	ft_strncpy(name, env_var, i);
	if (env_var[i] != 0)
		i++;
	value = ft_strdup(&env_var[i]);
	if (value == NULL)
			return (NULL);
	return (new_env_node(name, value));
}


int		tab_to_lst(char **env, t_environ **env_lst)
{
	int 		i;
	t_environ	*tmp;

	i = 0;
	
	while (env[i] != NULL)
	{
		tmp = env_to_lst(env[i]);
		if (tmp == NULL)
			return (MEMERR);
		add_node(env_lst, tmp);
		i++;
	}
	if (i == 0)
		*env = NULL;
	return (0);
}


t_environ	*get_env_node(char *name)
{
	size_t		len;
	t_environ	*tmp;

	len = 0;
	while ((name[len] != 0))
		len++;
	if (len == 0)
		return (NULL);
	tmp = *g_environ;
	while (tmp != NULL)
	{
		if ((!ft_strncmp(tmp->name, name, len))
		   		&& (ft_strlen(tmp->name) == len))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
 
char	*get_env_value(char *name)
{
	t_environ *tmp;
	char		*clean_name;	
	int			i;

	i = 0;
	while (valid_env_char(name[i]))
		i++;
	clean_name = ft_strnew(i);
	if (clean_name == NULL)
		return (NULL);
	ft_strncpy(clean_name, name, i);
	tmp = get_env_node(clean_name);
	free(clean_name);
	if (tmp != NULL)
		return (tmp->value);
	return (NULL);
}

int		set_shell_env(char *name, char *newvalue)
{
	t_environ *tmp;

	if (newvalue == NULL)
		newvalue = "";
	if ((tmp = get_env_node(name)) == NULL)
	{
		tmp = new_env_node(ft_strdup(name), ft_strdup(newvalue));
		if (tmp != NULL)
			add_node(g_environ, tmp);
		else
			return (MEMERR);			
	}
	else
	{
		if (tmp->value != NULL)
			free(tmp->value);
		tmp->value = ft_strdup(newvalue);	
		if (tmp->value == NULL)
			return (MEMERR);
	}
	return (0);
}
