/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:36:19 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/20 16:01:34 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_outside(int count)
{
	if (count % 2 == 0)
		return (1);
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
	if (s_count % 2 != 0)
	{
		printf("there is unclosed single quote.\n");
		return (1);
	}
	if (d_count % 2 != 0)
	{
		printf("there is unclosed double quote.\n");
		return (1);
	}
	return (0);
}