/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:19:46 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:17:24 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	int	i;

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
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	**dup_2d(char **str)
{
	char	**dupp;
	int		len;
	int		i;

	len = count_2d_len(str);
	i = 0;
	dupp = malloc(sizeof(char *) * (len + 1));
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
