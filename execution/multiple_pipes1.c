/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:20:38 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:42:51 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_int_arr(int **arr, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	pipes_helper(int i, t_pipes var)
{
	i = 0;
	while (i < var.pipes)
	{
		close(var.fd[i][0]);
		close(var.fd[i][1]);
		i++;
	}
	i = 0;
	while (i < var.pipes + 1)
		waitpid(var.pids[i++], &var.status, 0);
	g_sigs.process = 0;
	g_sigs.exit_s = WEXITSTATUS(var.status);
	free(var.pids);
	free_int_arr(var.fd, var.pipes);
}

void	pipes_helper1(int i, t_list *tmp, t_pipes var, t_list **env)
{
	if (i == 0)
		first_command(tmp, env, var.fd[i]);
	else if (i == var.pipes)
		last_command(tmp, env, var.fd[i - 1]);
	else
		middle_command(tmp, env, var.fd[i - 1], var.fd[i]);
}

void	pipes_helper2(int i, t_pipes var, t_list **tmp)
{
	if (i != 0)
	{
		close(var.fd[i - 1][0]);
		close(var.fd[i - 1][1]);
	}
	*tmp = (*tmp)->next;
}

void	multiple_pipes(t_list *commands, t_list **env)
{
	int		i;
	t_pipes	var;
	t_list	*tmp;

	i = 0;
	tmp = commands;
	var.pipes = count_pipes(commands);
	var.fd = malloc(sizeof(int *) * (var.pipes + 1));
	var.pids = malloc(sizeof(int) * (var.pipes + 1));
	while (i < var.pipes)
		var.fd[i++] = malloc(sizeof(int) * 2);
	var.fd[i] = NULL;
	i = 0;
	while (i < var.pipes + 1)
	{
		if (i < var.pipes && pipe(var.fd[i]) < 0)
			return ;
		var.pids[i] = fork();
		g_sigs.process = 1;
		if (var.pids[i] == 0)
			pipes_helper1(i, tmp, var, env);
		pipes_helper2(i, var, &tmp);
		i++;
	}
	pipes_helper(i, var);
}
