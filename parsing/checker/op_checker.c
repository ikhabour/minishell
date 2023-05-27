/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_checker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:16:16 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/27 18:01:10 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_redirec_op(char *input)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(input);
	if (input[i] == '>' || (input[i  + 1] && input[i] == '<' && input[i + 1] != '<'))
	{
		if (input[i + 2] && (input[i] == '>' && input[i + 1] != ' ' && input[i + 2] == '>'))
		{
			write (2, "syntax error near unexpected token `>'\n", 40);
			exit_s = 258;
			return (exit_s);
		}
	}
	while (input[i])
	{
		if (input[i + 2] && (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>'))
		{
			write (2, "syntax error near unexpected token `>'\n", 40);
			exit_s = 258;
			return (exit_s);
		}
		if (input[i + 2] && (input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<'))
		{
			write (2, "syntax error near unexpected token `>'\n", 40);
			exit_s = 258;
			return (1);
		}
		i++;
	}
	i--;
	if (input[i] == '>' || input[i] == '<')
	{
		write (2, "syntax error near unexpected token `newline'\n", 46);
		exit_s = 258;
		return (1);
	}
	return (0);
}
