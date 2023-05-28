/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 15:40:24 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/27 15:40:35 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_pipes(t_list *commands)
{
	int pipe;

	pipe = -1;
	while (commands)
	{
		pipe++;
		commands = commands->next;
	}
	return (pipe);
}

void	multiple_pipes(t_list *commands, t_list **env)
{
	int i;
	(void)env;
	i = 0;
	int *fd;

	fd = malloc(sizeof(int *) *(count_pipes(commands)));
	while (i <= count_pipes(commands))
	{
		fd[i] = (int)malloc(sizeof(int) * 2);
		printf("Successfully Allocated.\n");
		i++;
	}
	i = 0;
	while (i <= count_pipes(commands))
	{
		if (pipe(fd[i]) < 0)
			return ;
		i++;
	}
	return ;
}