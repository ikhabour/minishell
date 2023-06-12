/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:45:16 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/12 18:06:16 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	op_order(int *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == R_IN_OUT)
		{
			if (token[i + 1] && token[i + 1] != R_OUT_FILE)
			{
				if (token[i + 1] == R_IN_OUT)
					write (2, "Syntax error near unexpected token `<>'\n", 41);
				else if (token[i + 1] == R_IN_SIG)
					write (2, "Syntax error near unexpected token `<'\n", 40);
				else if (token[i + 1] == R_OUT_SIG)
					write (2, "Syntax error near unexpected token `>'\n", 40);
				else if (token[i + 1] == R_APP_SIG)
					write (2, "Syntax error near unexpected token `>>'\n", 41);
				else if (token[i + 1] == HEREDOC_SIG)
					write (2, "Syntax error near unexpected token `<<'\n", 41);
				free(token);
				return (1);
			}
		}
		if (token[i] == R_APP_SIG)
		{
			if (token[i + 1] && token[i + 1] != R_APP_FILE)
			{
				free(token);
				write (2, "Syntax error near unexpected token `newline'\n", 45);
				// printf("Error, problem with APP_SIG\n");
				return (1);
			}
		}
		if (token[i] == R_IN_SIG)
		{
			if (token[i + 1] && token[i + 1] != R_IN_FILE )
			{
				free(token);
				write (2, "Syntax error near unexpected token `newline'\n", 45);
				// printf("Error, problem with R_IN_SIG\n");
				return (1);
			}
		}
		if (token[i] == R_OUT_SIG)
		{
			if (token[i + 1] && token[i + 1] != R_OUT_FILE)
			{
				free(token);
				write (2, "Syntax error near unexpected token `newline'\n", 45);
				// printf("Error, problem with R_OUT_SIG\n");
				return (1);
			}
		}
		if (token[i] == HEREDOC_SIG)
		{
			if (token[i + 1] && token[i + 1] != HEREDOC_LIM)
			{
				free(token);
				write (2, "Syntax error near unexpected token `newline'\n", 45);
				// printf("Error, problem with Heredoc.\n");
				return (1);
			}
		}
		if (token[i] == PIPE)
		{
			if (token[i + 1] && token[i + 1] == PIPE)
			{
				free(token);
				printf("Error, problem with pipe.\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}