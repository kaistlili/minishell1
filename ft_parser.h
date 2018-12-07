
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*  ft_parsing.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 23:39:07 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 18:35:37 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PARSING_H
# define FT_PARSING_H

#include "minishell.h"

#define CMD 2
#define SEP 1
#define PIPE 0

typedef enum	e_node_type
{
	e_pipe, sep, cmd
}				t_node_type;

typedef struct	s_tree
{
	t_node_type		type;
	t_command		*data;
	struct s_tree	*left;
	struct s_tree	*right;
}				t_tree;
//tree building
int	ft_build_tree(char *line, t_tree **tree);
t_tree	*tr_new_node(int type);
int add_cmd(t_tree *node, t_tree **head);
int add_pipe(t_tree *node, t_tree **head);
int add_sep(t_tree *node, t_tree **head);
//syntax.c
int	assert_syntax(t_tree *head, int level);
// eval.c
int	eval_tree(t_tree *tree);
//util
void free_tree(t_tree *head);
#endif
