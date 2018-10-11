/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktlili <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 19:55:35 by ktlili            #+#    #+#             */
/*   Updated: 2018/09/11 20:01:54 by ktlili           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_wordcount(char const *s, char c)
{
	int i;
	int count;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			count++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
	}
	return (count);
}

static int	ft_wordlen(char const *s, char c)
{
	int i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
		i++;
	return (i);
}

char		**ft_strsplit(char const *s, char c)
{
	int		i;
	int		j;
	int		wordlen;
	char	**tab;

	i = 0;
	j = 0;
	tab = (char**)malloc(sizeof(char*) * (ft_wordcount(s, c)+ 1) );
	if (tab == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			wordlen = ft_wordlen(&s[i], c);
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
