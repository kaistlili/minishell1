/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 23:07:32 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/06 19:27:42 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_environ **g_environ = NULL;

void	dispatch_parse_err(int error)
{
	if (error == 1)
	{
		ft_printf("minishell: malloc error\n");
		exit(1);
	}
	else if (error == 2)
		ft_printf("minishell: syntax error\n");
}

static void		show_prompt(void)
{
	char *pwd;

	pwd = handle_pwd_l();
	if (pwd != NULL)
	{
		ft_printf("%s", pwd);
		free(pwd);
	}	
	write(1, "$> ", 3);
}

int		init_g_env(char **env)
{
	t_environ *shlvl;
	char *newlvl;
	int	 lvl;
	
	if (tab_to_lst(env, g_environ) != 0)
	{
		dispatch_parse_err(MEMERR);
		return (MEMERR);
	}
	if ((shlvl = get_env_node("SHLVL")) == NULL)
		return (set_shell_env("SHLVL","1"));
	lvl = ft_atoi(shlvl->value);
	if ((lvl < 0) || ((lvl + 1) < 0))
		lvl = 0;
	newlvl = ft_itoa(lvl + 1);
	if (newlvl == NULL)
		return (MEMERR);
	if (set_shell_env("SHLVL", newlvl) != 0)
	{
		free(newlvl);
		return (MEMERR);
	}
	free(newlvl);
	return (0);
}

int		exec_loop(t_command *command_lst)
{
	t_command	*tmp;

	tmp = command_lst;
	if ((tmp != NULL) && (tmp->args != NULL))
	{
		while (tmp != NULL) 
		{
			if ((execute_cmd(tmp)) == MEMERR)
			{
				free_cmdlst(command_lst);
				return (MEMERR);
			}
			tmp = tmp->next;
		}
	}
	return (0);
}

int	main(int ac, char **av, char **env)
{
	char 		*line;
	t_command	**command_lst;
	int			ret;

	(void)ac;
	(void)av;
	g_environ = ft_memalloc(sizeof(t_environ*));
	command_lst = ft_memalloc(sizeof(t_command*));
	if ((command_lst == NULL) || (g_environ == NULL) || (init_g_env(env) != 0))
		return (MEMERR);
	show_prompt();
	while (get_next_line(0, &line) > 0)
	{
		if ((ret = parser(line, command_lst)) != 0)
			dispatch_parse_err(ret);
		else if ((ret == MEMERR) 
				|| (ret = exec_loop(*command_lst) == MEMERR))
			break;
		free_cmdlst(*command_lst);
		free(line);	
		*command_lst = NULL;
		show_prompt();
	}
	free(line);
	free(command_lst);
	write(1, "\n", 1);
	return (ret);	
}
