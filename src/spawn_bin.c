/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn_bin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 15:11:09 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/06 18:11:46 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	void	exec_error(int errnum, char *str)
{
	if (errnum == 1)
		putstr_stderr("minishell: PATH not set");
	else if (errnum == 2)
		putstr_stderr("minishell: command not found: ");
	else if (errnum == 3)
		putstr_stderr("minishell: permission denied: ");
	else if (errnum == 4)
		putstr_stderr("minishell: no such file or directory: ");
	else if (errnum == 5)
		putstr_stderr("minishell: exec error: ");
	if (str != NULL)
		putstr_stderr(str);
	putstr_stderr("\n");
}

static	int bin_perm(char *path)
{
	struct stat target;

	if (stat(path, &target) != 0)
		return (4);
	if (!S_ISREG(target.st_mode))
		return(3);
	if (access(path, X_OK) != 0)
		return (3);
	return (0);
}

char	*test_access(char *path, char *binname, int *ret)
{
	char *tmp;

	tmp = ft_strjoin(path, binname);
	if (tmp == NULL)
		return (NULL);
	if ((ret[0] = bin_perm(tmp)) == 0)
		return (tmp);
	else if (ret[0] != 4)
		ret[1] = ret[0];
	free(tmp);
	return (NULL);
}

char 	*iter_paths(char **tab, char *binname, int *ret)
{
	char	*tmp;
	int		i;
	
	i = 0;
	while (tab[i] != NULL)
	{
		add_slash(&tab[i]);
		if ((tmp = test_access(tab[i], binname, ret)) != NULL)
		{
			free_tab(tab);
			return (tmp);
		}
		i++;
	}
	return (NULL);
}

char	*searchbin(char *binname, char *path)
{
	char	**tab;
	char	*tmp;
	int		ret[2];

	tab = NULL;
	ret[1] = 2;
	if (path != NULL)
		if ((tab = ft_strsplit(path, ':')) == NULL)
			return (NULL);
	if ((path == NULL) || (tab[0] == NULL))
	{
		if (bin_perm(binname) == 0)
		{
			if (tab != NULL)
				free_tab(tab);
			return (ft_strdup(binname));
		}
	}
	else if ((tmp = iter_paths(tab, binname, ret)) != NULL)
			return (tmp);
	exec_error(ret[1], binname);
	if (tab != NULL)
		free_tab(tab);
	return (NULL);
}

int	spawner(t_command *cmd, char *binpath)
{
	pid_t pid;
	
	pid = fork();
	if (pid == -1)
		return (MEMERR);
	if (pid == 0)
	{
		execve(binpath, cmd->args, cmd->process_env);
		putstr_stderr("minishell: bad file format\n");
		exit(1);
	}
	if (pid > 0)
	{
		wait(NULL);
	}
	return (0);
}

int		handle_fullpath(t_command *cmd, char **full_path)
{
	int ret;

	if ((ret = bin_perm(cmd->args[0])) != 0)
	{
		exec_error(ret, cmd->args[0]);
		return (ACCERR);
	}
	*full_path = ft_strdup(cmd->args[0]);
	if (*full_path == NULL)
		return (MEMERR);
	return (0);
}

int	spawn_bin(t_command *cmd)
{
	char	*full_path;
	char 	*bin_path;
 	int		ret;

	full_path = NULL;
	if ((ft_ispath(cmd->args[0])) 
			&& ((ret = handle_fullpath(cmd, &full_path)) != 0))
			return (ret);
	else if (!ft_ispath(cmd->args[0]))
	{
		bin_path = tab_get_value("PATH", cmd->process_env);
		if (bin_path == NULL)
			bin_path = get_env_value("PATH");
		full_path = searchbin(cmd->args[0], bin_path);	
		if (full_path == NULL) 
			return (0);
	}
	ret = spawner(cmd, full_path);
	free(full_path);
	return (ret);
}
