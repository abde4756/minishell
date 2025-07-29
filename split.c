/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:18:44 by aait-laf          #+#    #+#             */
/*   Updated: 2025/06/15 16:20:57 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


int	word_count(char const *s, char c)
{
	int	flag;
	int	word;

	flag = 0;
	word = 0;
	while (*s)
	{
		if (*s != c && flag == 0)
		{
			flag = 1;
			word++;
		}
		else if (*s == c && flag != 0)
			flag = 0;
		s++;
	}
	return (word);
}

char	*alloc_words(char const *s, char c)
{
	char	*p;
	int		len;

	len = 0;
	while (*s == c && *s)
		s++;
	while (s[len] != c && s[len])
		len++;
	p = malloc((len + 1) * sizeof(char));
	len = 0;
	while (s[len] && s[len] != c)
	{
		p[len] = s[len];
		len++;
	}
	p[len] = '\0';
	return (p);
}

char	**f_free(char **p)
{
	int	i;

	i = 0;
	while (p[i])
	{
		free(p[i]);
		i++;
	}
	free(p);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**p;
	int		i;
	int		flag;

	if (!s)
		return (NULL);
	p = (char **)malloc((word_count(s, c) + 1) * sizeof(char *));
	if (!p)
		return (NULL);
	i = 0;
	flag = 1;
	while (*s)
	{
		if (*s != c && flag == 1)
		{
			(1) && (flag = 0, p[i] = alloc_words(s, c));
			if (!p[i])
				return (f_free(p));
			i++;
		}
		else if (*s == c && flag == 0)
			flag = 1;
		s++;
	}
	return (p[i] = NULL, (p));
}