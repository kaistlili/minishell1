/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   changedir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 00:00:01 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/06 18:20:07 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_cdpath(char **curpath) /* this cant return void, we need error handling*/
{
	char	**cd_path_tab;
	char	*cd_path;
	int		ret;
	int 	i;

	cd_path = get_env_value("CDPATH");
	if ((cd_path == NULL) || (*cd_path == 0))
		return (0);
	cd_path_tab = ft_strsplit(cd_path, ':');
	if (cd_path_tab == NULL)
		return (MEMERR);
	i = 0;
	while (cd_path_tab[i] != NULL)
	{
		if ((add_slash(&cd_path_tab[i]) == MEMERR) 
			|| ((ret = dir_access(cd_path_tab[i], curpath)) == MEMERR))
		{
			free_tab(cd_path_tab);
			return (MEMERR);
		}
		if (ret == 0)
			break;
		i++;
	}
	free_tab(cd_path_tab);
	return (0);
}


int	handle_cdpath(char **curpath)
{
	if ((ft_strncmp(*curpath,"/", 1)) /*add exception for .. and . */ 
			&& (ft_strncmp(*curpath, "../", 3))
				&& (ft_strncmp(*curpath, "./", 2)))
	{
		if (check_cdpath(curpath) == MEMERR)
		{
			free(*curpath);
			return (MEMERR); 
		}
	}
	return (0);
}

static int	cd_error(int errnum, char *str)
{
	if (errnum == 1)
		putstr_stderr("cd: Pathname too long");
	else if (errnum == 2)
		putstr_stderr("cd : no such file or directory: ");
	else if (errnum == 3)
		putstr_stderr("cd: permission denied: ");
	else if (errnum == 4)
		putstr_stderr("cd: HOME not set");
	else if (errnum == 5)
		putstr_stderr("cd: Not a directory: ");
	if (str != NULL)
		putstr_stderr(str);
	putstr_stderr("\n");
	return (0);
}

char	*handle_pwd_l(void)
{
	char *env_pwd;
	char *p_pwd;
	char *ret;

	env_pwd = get_env_value("PWD");
	p_pwd = getcwd(NULL, 0);
	if ((p_pwd == NULL) && (env_pwd == NULL))
		return (NULL);
	if ((env_pwd != NULL) && (!path_access(env_pwd)))
	{
		ret = ft_strdup(env_pwd);
		if (ret == NULL)
			return (p_pwd);
		if (p_pwd != NULL)
			free(p_pwd);
		return (ret);
	}
	return (p_pwd);
}

void	update_env_pwd(char *pwd, char *curpath)
{
	
	if (pwd != NULL)
	{	
		set_shell_env("OLDPWD", pwd);
		set_shell_env("PWD", curpath);
	}
	else
	{
		pwd = getcwd(NULL, 0);
		if (pwd != NULL)
		{
			set_shell_env("PWD", pwd);
			free(pwd);
		}
	}
}

void	cd_dispatch_err(char *arg, char *curpath)
{
	int ret;

	if (arg == NULL)
		arg = curpath;
	if ((ret =  path_access(curpath)) != 0) 
		cd_error(ret, arg);
}

void	canon_form(char *curpath)
{
	cleanpath(curpath);  
	cleandotdot(curpath);
}

int	cd_l(char *curpath, char *arg)
{
	char 	*pwd;
	char 	*tmp;

	pwd = handle_pwd_l();
	if ((curpath[0] != '/') && (pwd != NULL)) // if getcwd fails we never get here
	{
		if (add_slash(&pwd) != 0)
			return (MEMERR);
		tmp = curpath;
		curpath = ft_strjoin(pwd, curpath);
		free(tmp);
		if (curpath == NULL)
		{
			free(pwd);
			return (MEMERR);
		}
	}
	canon_form(curpath);
	if (chdir(curpath) != 0)
		cd_dispatch_err(arg, curpath);
	else
		update_env_pwd(pwd, curpath);
	if (pwd != NULL)
		free(pwd);
	free(curpath);
	return (0);
}

int	cd_p(char *curpath, char *arg)
{
	char 	*tmp_pwd;
	char 	*old_pwd;

	tmp_pwd = get_env_value("PWD");
	if (chdir(curpath) != 0)
	{
		cd_dispatch_err(arg, curpath);
		return (0);
	}
	old_pwd = getcwd(NULL, 0);
	if (tmp_pwd != NULL) 	
		set_shell_env("OLDPWD", tmp_pwd);
	else
		set_shell_env("OLDPWD", old_pwd);
	if (old_pwd != NULL)
		free(old_pwd);
	tmp_pwd = getcwd(NULL, 0 );	
	set_shell_env("PWD", tmp_pwd);
	if (tmp_pwd != NULL)
		free(tmp_pwd);	
	free(curpath);
	return (0);
}

char *getoldpwd(void)
{
	char *oldpwd;

	oldpwd = get_env_value("OLDPWD");
	if (oldpwd == NULL)
		return ("\0");
	return (oldpwd);
}

char	cd_parseopt(char **args)
{
	int		flag;
	char	opt;

	opt = 'L';
	g_optind = 1;
	while ((flag = ft_getopt(args,"PL")) != -1)
	{
		opt = (char)flag;
		if (opt == '?')
		{
			ft_printf("usage: cd [-LP] [dir]\n");
			break;
		}
	}
	return (opt);
}

int		change_dir(t_command *cmd)
{
	char	*curpath;
	char	opt;

	if ((opt = cd_parseopt(cmd->args)) == '?')
		return (0);
	if ((cmd->args[g_optind] == NULL) || (cmd->args[g_optind][0] == 0))
	{
		curpath = get_env_value("HOME");
		if ((curpath == NULL) || (*curpath == 0))
			return (cd_error(4, NULL));
		curpath = ft_strdup(curpath);
	}
	else if (!ft_strncmp(cmd->args[g_optind], "-", 2))
		curpath = ft_strdup(getoldpwd());
	else
		curpath = ft_strdup(cmd->args[g_optind]);	
	if ((curpath == NULL) || (handle_cdpath(&curpath) == MEMERR))
		return (MEMERR);
	if (ft_strlen(curpath) > PATH_MAX)
		return (cd_error(1, NULL));
	if (opt == 'P')
		cd_p(curpath, cmd->args[g_optind]);
	else if (opt == 'L')
		cd_l(curpath, cmd->args[g_optind]);
	return (0);
}
