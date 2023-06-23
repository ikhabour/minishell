/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:49:46 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:52:08 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*input_get(char *input)
{
	input = fill_line(input);
	input = add_spaces(input);
	return (input);
}

int	command_valid(t_vars var, char *input)
{
	if (valid_command(var.new))
	{
		free(var.arr);
		free_2d(var.new);
		free(input);
		return (1);
	}
	return (0);
}

void	my_allfree(char *input, t_vars var)
{
	free_2d(var.new);
	free(var.arr);
	free(input);
}

void	close_free(t_vars var, char *input)
{
	close_files(var.commands);
	my_free(var.commands);
	free_all(input, var.new);
	free(var.arr);
}

int	executer(t_vars var, char *input, t_list **env)
{
	if (!var.commands)
	{
		my_allfree(input, var);
		return (1);
	}
	while (var.tmp && g_sigs.execc)
	{
		if (is_heredoc(var.tmp))
			here_docc(var.tmp, *env);
		var.tmp = var.tmp->next;
	}
	if (ft_lstsize(var.commands) > 1)
	{
		multiple_pipes(var.commands, env);
		close_free(var, input);
		return (1);
	}
	if (execute_builtins(var.commands, env))
	{
		close_free(var, input);
		return (1);
	}
	return (0);
}
