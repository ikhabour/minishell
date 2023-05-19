/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:45:16 by bhazzout          #+#    #+#             */
/*   Updated: 2023/04/30 10:32:55 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	op_order(int *token)
{
	int	i;

	i = 0;
	while (token[i])
	{
		if (token[i] == R_APP_SIG)
		{
			if (token[i + 1] && token[i + 1] != R_APP_FILE)
			{
				printf("Error, problem with APP_SIG\n");
				return ;
			}
		}
		if (token[i] == R_IN_SIG)
		{
			if (token[i + 1] && token[i + 1] != R_IN_FILE)
			{
				printf("Error, problem with R_IN_SIG\n");
				return ;
			}
		}
		if (token[i] == R_OUT_SIG)
		{
			if (token[i + 1] && token[i + 1] != R_OUT_FILE)
			{
				printf("Error, problem with R_OUT_SIG\n");
				return ;
			}
		}
		if (token[i] == HEREDOC_SIG)
		{
			if (token[i + 1] && token[i + 1] != HEREDOC_LIM)
			{
				printf("Error, problem with Heredoc.\n");
				return ;
			}
		}
		if (token[i] == PIPE)
		{
			if (token[i + 1] && (token[i + 1] == R_APP_SIG || token[i + 1] == R_IN_SIG || token[i + 1] == R_OUT_SIG || token[i + 1] == HEREDOC_SIG || token[i + 1] == PIPE))
			{
				printf("Error, problem with pipe.\n");
				return ;
			}
		}
		i++;
	}
}