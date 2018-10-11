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
	int		i;
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

char	*expand_dollar(char *line)
{
	t_list	buffer;
	char	*e_var;
	int		i;
	int		j;

	i = 0;
	j = 0;
	buffer.content = ft_strnew(ft_strlen(line));
	if (buffer.content == NULL)
		return (NULL);
	buffer.content_size = ft_strlen(line);
	while (line[i] != 0)
	{
		if ((line[i] == '$') && (line[i + 1] != 0))
		{
			e_var = get_env_value(&line[i + 1]);
			if ((e_var != NULL) && (*e_var != 0))
			{
				if (putstr_dbuff(&buffer, e_var, &j) != 0)
					return (NULL);
			}
			while (valid_env_char(line[i + 1]))
				i++;
		}
		else
		{
			if (write_dbuff(&buffer, line[i], j) != 0)
				return (NULL);
			j++;
		}
		i++;
	}
	return (buffer.content);
}

int		expand_tokens(char **token)
{
	int		read;
	int		write;
	char	*tmp;

	read = 0;
	write = 0;
	while (token[read] != NULL)
	{
		if ((token[read][0] == '~') && (tilde_valid(&token[read][1])))
		{
			tmp = get_env_value("HOME");
			if (tmp != NULL)
				tmp = tilde_expand(tmp, token[read]);
			else
				tmp = ft_strdup("\0");
			if (tmp == NULL)
				return (MEMERR);		
			free(token[write]);
			token[write] = tmp;
		}
		tmp = expand_dollar(token[read]);
		if (tmp == NULL)
			return (MEMERR);
		free(token[write]);
		token[write] = tmp;
		if (token[write][0] != 0)
			write++;
		read++;
	}
	if (token[write] != NULL)
		free_tab_bytes(&token[write]);
	token[write] = NULL;
	return (0);
}
