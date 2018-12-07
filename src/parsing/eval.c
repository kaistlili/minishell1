#include "minishell.h"
#include "ft_parser.h"


int	pipe_callback(t_tree *leftn, t_tree *rightn)
{
	int pipes[2];
	int pid;

	if (pipe(pipes))
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		dup2(pipes[1], STDOUT_FILENO);
		close(pipes[0]);
		if (leftn->type == PIPE)
			pipe_callback(leftn->left, leftn->right);	
		execute_cmd(leftn->data);
		exit(0);
	}
	dup2(pipes[0], STDIN_FILENO);
	close(pipes[1]);
	wait(NULL);
	execute_cmd(rightn->data);
	exit(0);
	return (0);
}

int	eval_pipe(t_tree *tree)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		pipe_callback(tree->left, tree->right);
//		ft_printf("Pipe succedded!");
		exit(0);
	}
	//printf("kid's pid: %d", pid);
	wait(NULL);
	return (0);
}


int	eval_tree(t_tree *tree)
{
	if (tree->type == PIPE)
	{
		eval_pipe(tree);
		return (1);
	}
	else if (tree->type == SEP)
	{
		eval_tree(tree->left);
		eval_tree(tree->right);
	}
	else if (tree->type == CMD)
		execute_cmd(tree->data);
	return (1);
}
