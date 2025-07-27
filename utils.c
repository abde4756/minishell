/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-laf <aait-laf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:18:47 by aait-laf          #+#    #+#             */
/*   Updated: 2025/06/22 15:10:31 by aait-laf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishel.h"

char	*ft_strjoin(char *dest, char *src, char c)
{
	char	*p;
	size_t	i;
	size_t	j;

	if (!dest || !src)
		return (NULL);
	i = 0;
	j = 0;
	p = malloc(ft_strlen((char *)dest) + ft_strlen((char *)src) + 1);
	if (!p)
		return (NULL);
	while (dest[i] != '\0')
	{
		p[i] = dest[i];
		i++;
	}
	p[i] = c;
	while (src[j] != '\0')
	{
		p[i + j] = src[j];
		j++;
	}
	p[i + j] = '\0';
	return (p);
}

void    print_str(char *str)
{
    int     i;

    i = 0;
    while (str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
}

int	ft_isalnum(int c)
{
	return (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'Z'))
		|| ((c >= 'a') && (c <= 'z')));
}

int	ft_isalpha(int c)
{
	return (!(c > 122 || (c < 97 && c > 90) || c < 65));
}

int	ft_isdigit(int c)
{
	return ((c >= '0') && (c <= '9'));
}