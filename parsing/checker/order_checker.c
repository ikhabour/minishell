/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:45:16 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 03:34:46 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	r_in_out(int *token, int i)
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
		return (1);
	}
	return (0);
}

int	op_order_1(int *token, int i)
{
	if (token[i] == R_APP_SIG)
	{
		if (token[i + 1] && token[i + 1] != R_APP_FILE)
		{
			write (2, "Syntax error near unexpected token `newline'\n", 45);
			return (1);
		}
	}
	if (token[i] == R_IN_SIG)
	{
		if (token[i + 1] && token[i + 1] != R_IN_FILE)
		{
			write (2, "Syntax error near unexpected token `newline'\n", 45);
			return (1);
		}
	}
	if (token[i] == R_OUT_SIG)
	{
		if (token[i + 1] && token[i + 1] != R_OUT_FILE)
		{
			write (2, "Syntax error near unexpected token `newline'\n", 45);
			return (1);
		}
	}
	return (0);
}

int	op_order_2(int *token, int i)
{
	if (token[i] == HEREDOC_SIG)
	{
		if (token[i + 1] && token[i + 1] != HEREDOC_LIM)
		{
			write (2, "Syntax error near unexpected token `newline'\n", 45);
			return (1);
		}
	}
	if (token[i] == PIPE)
	{
		if (token[i + 1] && token[i + 1] == PIPE)
		{
			printf("syntax error near unexpected token `|'\n");
			return (1);
		}
	}
	return (0);
}

int	op_order(int *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == R_IN_OUT)
		{
			if (r_in_out(token, i))
				return (1);
		}
		if (op_order_1(token, i))
			return (1);
		if (op_order_2(token, i))
			return (1);
		i++;
	}
	return (0);
}
