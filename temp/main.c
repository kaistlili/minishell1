/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 17:59:42 by ktlili            #+#    #+#             */
/*   Updated: 2018/09/09 23:02:31 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./lib/libft.h"

extern char **environ;

void func()
{
	int i;

	i = 0;
	while (environ[i] != NULL)
	{
		ft_printf("%s\n", environ[i]);
		i++;
	}
}

int	add_env(char *name, char *value)
{
	int i;
	int k;

	i = 0;
	while (environ[i] != NULL)
	{
		ft_printf("%d", i);
		i++;
	}
	ft_printf("setenv here\n");
	
	k = i + 100;
	while (i < k)
	{
		environ[i] = malloc(10);
		ft_strncpy(environ[i],"LOL=TOTO",8);
		environ[i][8] = 0;
		i++;
	}
	return (1);	
}
int main(int ac, char **av, char **env)
{
	ft_printf("env before: %p env[0] %p\n", environ, environ[0]);
	//	func();
	putenv("TOTO=TATA");
	putenv("LULZ=LALA");
	ft_printf("\n\n\n\nenv after: %p env[0] %p\n", environ, environ[0]);
	func();
}
