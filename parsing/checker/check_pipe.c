/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:13:39 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 02:03:04 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	start(char *input)
{
	(void) input;
	write (2, "syntax error near unexpected token `|'\n", 40);
	g_sigs.exit_s = 1;
}

void	mid(char *input)
{
	(void) input;
	write (2, "syntax error near unexpected token `||'\n", 40);
	g_sigs.exit_s = 127;
}

int	end(char *input, int i)
{
	if (input[i] == '|')
	{
		write(2, "syntax error: unexpected end of file\n", 38);
		g_sigs.exit_s = 1;
		return (1);
	}
	return (0);
}

int	check_pipe(char *input)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (input[i] == '|')
	{
		start(input);
		return (1);
	}
	while (input[i] && input[i + 1])
	{
		if (input[i] == '\'' || input[i] == '"')
			flag = is_outside(flag, input[i]);
		if ((input[i] == '|' && input[i + 1] == '|') && flag == 0)
		{
			mid(input);
			return (1);
		}
		i++;
	}
	if (end(input, i))
		return (1);
	return (0);
}
