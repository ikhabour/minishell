/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 23:56:49 by bhazzout          #+#    #+#             */
/*   Updated: 2023/04/27 10:27:34 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	char			*str;
	unsigned char	ch;
	int				i;

	ch = (unsigned char)c;
	i = 0;
	str = (char *)s;
	while (str[i])
	{
		if (str[i] == ch)
			return (&str[i]);
		i++;
	}
	if (!ch)
		return (&str[i]);
	return (0);
}

char	*ft_strdup(const char *s1)
{
	size_t	l;
	char	*str;

	l = ft_strlen(s1) + 1;
	str = malloc(l);
	if (str)
	{
		ft_memcpy(str, s1, l);
	}
	return (str);
}

void	ft_bzero(void *s, size_t n)
{
	char	*ptr;
	size_t	i;

	ptr = (char *)s;
	i = 0;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t	len;
	char	*ptr;

	if (size && count > SIZE_MAX / size)
		return (0);
	len = count * size;
	ptr = (char *)malloc(len);
	if (!ptr)
		return (0);
	ft_bzero (ptr, len);
	return (ptr);
}
