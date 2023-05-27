/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:13:39 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/27 17:48:02 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int check_pipe(char *input)
{
	int i;
	int	flag;

	i = 0;
	flag = 0;
	if (input[i] == '|')
	{
		write (2, "syntax error near unexpected token `|'\n", 40);
		exit_s = 1;
		return (1);
	}
	while (input[i] && input[i + 1])
	{
		if (input[i] == '\'' || input[i] == '"')
			flag = is_outside(flag, input[i]);
		if ((input[i] == '|' && input[i + 1] == '|') && flag == 0)
		{
			// printf("Error, double pipe.\n");
			write (2, "command not found\n", 19);
			exit_s = 127;
			return (1);
		}
		i++;
	}
	if (input[i] == '|')
	{
		// printf("Error, | at the end of the line.\n");
		exit_s = 1;
		return (1);
	}
	return (0);
}