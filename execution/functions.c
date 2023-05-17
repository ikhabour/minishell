/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:19:46 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/17 20:51:49 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlenn(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_strdupp(char *str)
{
	char	*string;
	int		i;

	i = 0;
	string = malloc(sizeof(char) * (ft_strlenn(str) + 1));
	if (!string)
		return (NULL);
	while (str[i])
	{
		string[i] = str[i];
		i++;
	}
	string[i] = '\0';
	return (string);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!n)
		return (0);
	while (s1[i] && s2[i] && (s1[i] == s2[i]) && i < n)
	{
		i++;
	}
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strjoinn(char *s1, char *s2)
{
	int		i;
	char	*new;
	int		j;

	if (!s1 || !s2)
		return (0);
	i = 0;
	j = ft_strlenn(s1);
	new = malloc(ft_strlenn(s1) + ft_strlenn(s2) * sizeof(char) + 1);
	if (new == NULL)
		return (NULL);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
		new[j++] = s2[i++];
	new[j] = '\0';
	return (new);
}

int	ft_atoi(char *str)
{
	long long	result;
	int			sign;
	int			i;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i++] - '0';
		if (result > 2147483647 && sign == 1)
			return (-1);
		if (result < -2147483647 && sign == -1)
			return (0);
	}
	return (result * sign);
}

int	is_digit(char *str)
{
	int i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		i++;
	if (!str[i])
		return (1);
	return (0);
}


int	count_2d_len(char **str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	**dup_2d(char **str)
{
	char **dupp;
	int		len;
	int		i;

	len = count_2d_len(str);
	i = 0;
	dupp = malloc(sizeof(char *) *(len + 1));
	if (!dupp)
		return (NULL);
	while (str[i])
	{
		dupp[i] = ft_strdupp(str[i]);
		i++;
	}
	dupp[i] = NULL;
	return (dupp);
	
}