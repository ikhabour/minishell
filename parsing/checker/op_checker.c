/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_checker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:16:16 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 02:03:04 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	op_start(char *input, int i)
{
	if (input[i] == '>' || (input[i + 1]
			&& input[i] == '<' && input[i + 1] != '<'))
	{
		if (input[i] && input[i + 1] && input[i + 2]
			&& (input[i] == '>' && input[i + 1] != ' ' && input[i + 2] == '>'))
		{
			write (2, "syntax error near unexpected token `>'\n", 40);
			g_sigs.exit_s = 258;
			return (1);
		}
	}
	return (0);
}

int	op_end(char *input, int i)
{
	if (input[i] == '>' || input[i] == '<')
	{
		if (input[i] == '<')
		{
			write (2, "syntax error near unexpected token `<'\n", 39);
			g_sigs.exit_s = 258;
			return (1);
		}
		else
		{
			write (2, "syntax error near unexpected token `newline'\n", 46);
			g_sigs.exit_s = 258;
			return (1);
		}
	}
	return (0);
}

int	op_mid(char *input, int i)
{
	if (input[i] && input[i + 1] && input[i + 2] && (input[i] == '>'
			&& input[i + 1] == '>' && input[i + 2] == '>'))
	{
		write (2, "syntax error near unexpected token `>'\n", 40);
		g_sigs.exit_s = 258;
		return (1);
	}
	if (input[i] && input[i + 1] && input[i + 2] && (input[i] == '<'
			&& input[i + 1] == '<' && input[i + 2] == '<'))
	{
		write (2, "syntax error near unexpected token `<'\n", 40);
		g_sigs.exit_s = 258;
		return (1);
	}
	return (0);
}

int	check_redirec_op(char *input)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(input);
	if (op_start(input, i))
		return (1);
	while (input[i])
	{
		if (op_mid(input, i))
			return (1);
		i++;
	}
	i--;
	if (op_end(input, i))
		return (1);
	return (0);
}
