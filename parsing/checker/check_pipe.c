/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:13:39 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/20 16:01:34 by ikhabour         ###   ########.fr       */
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
		printf("Error, command line starts with a |.\n");
		return (1);
	}
	while (input[i] && input[i + 1])
	{
		if (input[i] == '\'' || input[i] == '"')
			flag = is_outside(flag, input[i]);
		if ((input[i] == '|' && input[i + 1] == '|') && flag == 0)
		{
			printf("Error, double pipe.\n");
			return (1);
		}
		i++;
	}
	if (input[i] == '|')
	{
		printf("Error, | at the end of the line.\n");
		return (1);
	}
	return (0);
}