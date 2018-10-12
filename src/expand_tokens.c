/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/05 18:14:11 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 19:07:20 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		tilde_valid(char *str)
{
	if (*str == 0)
		return (1);
	if (*str == '/')
		return (1);
	return (0);
}

char	*tilde_expand(char *homepath, char *arg)
{
	size_t	tot_len;
	char	*expand;

	tot_len = ft_strlen(homepath) + ft_strlen(arg);
	expand = ft_memalloc(tot_len + 1);
	if (expand == NULL)
		return (NULL);
	ft_strcpy(expand, homepath);
	if (arg[0] == '~')
		arg++;
	ft_strcpy(expand + ft_strlen(homepath), arg);
	return (expand);
}

int	init_dbuff(t_list *buffer, char *line)
{
	buffer->content = ft_strnew(ft_strlen(line));
	if (buffer->content == NULL)
		return (MEMERR);
	buffer->content_size = ft_strlen(line);
	return (0);
}

char	*expand_dollar(char *line)
{
	t_list	buffer;
	char	*e_var;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (init_dbuff(&buffer, line) != 0)
		return (NULL);
	while (line[i] != 0)
	{
		if ((line[i] == '$') && (line[i + 1] != 0))
		{
			e_var = get_env_value(&line[i + 1]);
			if (((e_var != NULL) && (*e_var != 0))
				&& (putstr_dbuff(&buffer, e_var, &j) != 0))
				return (NULL);
			while (valid_env_char(line[i + 1]))
				i++;
		}
		else if (write_dbuff(&buffer, line[i], &j) != 0)
				return (NULL);
		i++;
	}
	return (buffer.content);
}

int		handle_tilde(char **arg, char **tmp, int write, int read)
{
	*tmp = get_env_value("HOME");
	if (*tmp != NULL)
		*tmp = tilde_expand(*tmp, arg[read]);
	else
		*tmp = ft_strdup("\0");
	if (*tmp == NULL)
		return (MEMERR);		
	free(arg[write]);
	arg[write] = *tmp;
	return (0);
}

int		expand_tokens(char **arg)
{
	int		read;
	int		write;
	char	*tmp;

	read = 0;
	write = 0;
	while (arg[read] != NULL)
	{
		if ((arg[read][0] == '~') && (tilde_valid(&arg[read][1])))
		{
			if (handle_tilde(arg, &tmp, write, read) != 0)
				return (MEMERR);
		}
		tmp = expand_dollar(arg[read]);
		if (tmp == NULL)
			return (MEMERR);
		free(arg[write]);
		arg[write] = tmp;
		if (arg[write][0] != 0)
			write++;
		read++;
	}
	if (arg[write] != NULL)
		free_tab_bytes(&arg[write]);
	arg[write] = NULL;
	return (0);
}
