/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 22:32:32 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/06 17:31:14 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	n_calculator(int num)
{
	long long	n;	
	int			i;

	n = (long long)num;
	if (n == 0)
		return (1);
	i = 0;
	if (n < 0)
	{
		i++;
		n *= -1;
	}
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int num)
{
	long long	n;
	char		*str;
	int			number;

	n = (long long)num;
	number = n_calculator(n);
	str = (char *)malloc(number + 1);
	if (!str)
		return (0);
	str[number] = '\0';
	if (n == 0)
		str[0] = '0';
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	while (n > 0)
	{	
		str[number - 1] = (n % 10) + '0';
		n = n / 10;
		number--;
	}
	return (str);
}