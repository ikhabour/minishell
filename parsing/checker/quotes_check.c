/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:36:19 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 02:03:04 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_outside(int count)
{
	if (count % 2 == 0)
		return (1);
	return (0);
}

int	count_checker(int s_count, int d_count)
{
	if (s_count % 2 != 0)
	{
		write(2, "syntax error, unclosed quotes\n", 31);
		g_sigs.exit_s = 1;
		return (1);
	}
	if (d_count % 2 != 0)
	{
		write(2, "syntax error, unclosed quotes\n", 31);
		g_sigs.exit_s = 1;
		return (1);
	}
	return (0);
}

int	check_quotes(char *input)
{
	int	i;
	int	d_count;
	int	s_count;

	d_count = 0;
	s_count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '"')
		{
			if (check_outside(s_count) == 1)
				d_count++;
		}
		if (input[i] == '\'')
		{
			if (check_outside(d_count) == 1)
				s_count++;
		}
		i++;
	}
	if (count_checker(s_count, d_count))
		return (1);
	return (0);
}
