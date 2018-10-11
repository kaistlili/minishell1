/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 19:55:35 by ktlili            #+#    #+#             */
/*   Updated: 2018/10/05 17:02:19 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_isblank(char c)
{
	if ((c == ' ') || (c == '\t') || (c == '\r'))
		return (1);
	return (0); 
}

static int	ft_wordcount(char const *s)
{
	int i;
	int count;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (ft_isblank(s[i])) 
			i++;
		else
		{
			count++;
			while ((!ft_isblank(s[i])) && (s[i] != '\0'))
				i++;
		}
	}
	return (count);
}

static int	ft_wordlen(char const *s)
{
	int i;

	i = 0;
	while ((!ft_isblank(s[i])) && (s[i] != '\0'))
		i++;
	return (i);
}

char		**ft_splitblanks(char const *s)
{
	int		i;
	int		j;
	int		wordlen;
	char	**tab;

	i = 0;
	j = 0;
	tab = (char**)malloc(sizeof(char*) * (ft_wordcount(s) + 1));
	if (tab == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		if (ft_isblank(s[i]))
			i++;
		else
		{
			wordlen = ft_wordlen(&s[i]);
			tab[j] = (char *)malloc(1 + (sizeof(char) * wordlen));
			tab[j][wordlen] = 0;
			ft_strncpy(tab[j], &s[i], wordlen);
			j++;
			i = i + wordlen;
		}
	}
	tab[j] = 0;
	return (tab);
}
