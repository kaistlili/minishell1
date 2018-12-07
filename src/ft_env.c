/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 13:58:14 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/06 18:17:29 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	valid_env_var(char *str)
{
	
	while (*str)
	{
		if (*str == '=')
			return (1);
		str++;
	}
	return (0);
}

int		merge_env(char **new_env, char *to_add)
{
	int len;

	len = 0;
	while (valid_env_char(to_add[len]))
		len++;
	while (*new_env != NULL)
	{
		if (!ft_strncmp(*new_env, to_add, len))
		{
			free(*new_env);
			*new_env = ft_strdup(to_add);
			if (*new_env == NULL)
				return (MEMERR);
			return (0);
		}
		new_env++;
	}
	return (-1);
}

int		append_tab(char **new_env, char **to_add, int count)
{
	int i;
	int ret;

	i = 0;
	while (new_env[i] != NULL)
		i++;
	while (count)
	{
		if ((ret = merge_env(new_env, *to_add)) == -1)
		{
			new_env[i] = ft_strdup(*to_add);
			if (new_env[i] == NULL)
				return (MEMERR);
			i++;
		}
		if (ret == MEMERR)
			return (MEMERR);
		count--;
		to_add++;
	}
	new_env[i] = NULL;
	return (0);
}

char	env_parseopt(char **args)
{
	int 	flag;
	char	opt;

	flag = 0;
	g_optind = 1;
	opt = 0;
	while ((flag = ft_getopt(args, "i")) != -1)
	{
		opt = (char)flag;
		if (opt == '?')
		{
			ft_printf("usage: env [-i] [NAME=VAR] .. [BIN]\n");
			break;
		}
	}
	return (opt);
}

char **construct_env(t_command *cmd, char opt, int count)
{
	char		**new_env;
	t_environ	*base_env;
	
	count = count - g_optind;
	base_env = *g_environ;
	if (opt == 'i')
		base_env = NULL;
	new_env = lst_to_tab(base_env, count);
	if ((new_env == NULL) 
			|| (append_tab(new_env, cmd->args + g_optind, count) == MEMERR))
		return (NULL);	
	return (new_env);
}

int	spawn_new_env(char **args, char **new_env)
{
	t_command	*new_cmd;
	int			ret;
(void)args;	
//	new_cmd = new_cmd_node(args);
	new_cmd = NULL;
	if (new_cmd == NULL)
		return (MEMERR);
	new_cmd->process_env = new_env;
	ret = spawn_bin(new_cmd);
	free_cmdlst(new_cmd);
	return (ret);
}

int	ft_env(t_command *cmd)
{
	int 		count;
	char		**new_env;
	char		opt;
	int			ret;

	ret = 0;
	if ((opt = env_parseopt(cmd->args)) == '?')
		return (0);
	count = g_optind;
	while ((cmd->args[count] != NULL) && (valid_env_var(cmd->args[count])))
		count++;
	if ((new_env = construct_env(cmd, opt, count)) == NULL)
		return (MEMERR);
	if (cmd->args[count] == NULL)
	{
		print_tab(new_env);
		free_tab(new_env);
	}
	else
		ret = spawn_new_env(cmd->args + count, new_env);
	return (ret);	
}
