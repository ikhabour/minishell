/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_checker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:16:16 by bhazzout          #+#    #+#             */
/*   Updated: 2023/04/29 17:19:31 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_redirec_op(char *input)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(input);
	if (input[i] == '>' || (input[i  + 1] && input[i] == '<' && input[i + 1] != '<'))
	{
		if (input[i] == '>' && input[i + 1] == '>')
			printf("Error, the command line starts with >>.\n");
		if (input[i] == '>' && input[i + 1] != '>')
			printf("Error, the command line starts with >.\n");
		return (1);
	}
	while (input[i])
	{
		if (input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>')
		{
			printf("Error, problem with > operator.\n");
			return (1);
		}
		if (input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<')
		{
			printf("Error, problem with < operator.\n");
			return (1);
		}
		i++;
	}
	i--;
	if (input[i] == '>' || input[i] == '<')
	{
		printf("Error, %c at the end of the command line.\n", input[i]);
		return (1);
	}
	return (0);
}
