/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 23:39:07 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 18:35:37 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lib/libft.h"
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <limits.h>

# define MEMERR 1
# define SYNERR 2
# define ENVERR 3
# define ACCERR 4
# define INCOMP 5
typedef	struct			s_command
{
	char 				**args;
	size_t				agcount;
	char 				**process_env;
	struct	s_command	*next;
	struct	s_command	*previous;
}						t_command;

typedef	struct			s_environ
{
	char				*name;
	char				*value;
	struct	s_environ	*next;
	struct	s_environ	*previous;
}						t_environ;

typedef	int	(*t_builtin)(t_command*);

extern	t_environ **g_environ;

//Tmp test functions
void		print_tab(char **tab);
void		print_cmd(t_command command);
void		print_cmd_lst(t_command *command);
void		print_env_lst(t_environ *lst);

// util
int			ft_cmptab(char **tab, char *str);
void		free_tab(char **tab);
void		free_tab_bytes(char **tab);
char		**dup_tab(char **tab);
char		**dup_ntab(char **tab, size_t n);
t_command	*new_cmd_node(char *cmd_line);
void		add_cmdlst(t_command **head, t_command *to_add);
void		free_cmdlst(t_command *command_lst);
void		putstr_stderr(char *str);
int			write_dbuff(t_list *buffer, char c, int *index);
int putstr_dbuff(t_list *buffer, char *str, int *index);
int			path_access(char *path);
int			ft_ispath(char *str);
char		*handle_pwd_l(void);

//env lst fn
int	tab_to_lst(char **env, t_environ **env_lst);
char **lst_to_tab(t_environ *env_lst, int size);

//env fn
char		*get_env_value(char *name);
int			set_shell_env(char *name, char *value);
int			valid_env_name(char *str);
int			valid_env_char(char c);
t_environ	*get_env_node(char *name);
void		delete_env_node(t_environ *to_del);
char		*tab_get_value(char *name, char **env);
//parsing
int			parser(char *line, t_command **cmd_lst);
int			expand_tokens(char **arg);

//executor
int			execute_cmd(t_command *cmd);
int			spawn_bin(t_command *cmd);
//cd
int 		dir_access(char *cd_path, char **curpath);
void		cleanpath(char *str);
int			add_slash(char **path);
void		cleandotdot(char *path);
//builtins
int			ft_echo(t_command *cmd);
int 		change_dir(t_command *cmd);
int			setenv_wrapper(t_command *cmd);
int			ft_unsetenv(t_command *cmd);
int			ft_env(t_command *cmd);
int			ft_env(t_command *cmd);
#endif
