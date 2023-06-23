/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:09:21 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:09:31 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_echo_option(char *str, t_bvars *var)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		if (!str[i])
			return (1);
		while (str[i])
		{
			if (str[i] == 'n')
				i++;
			else
				return (1);
		}
		var->n++;
	}
	return (0);
}

void	init_bvars(t_bvars *var)
{
	var->i = 0;
	var->j = 0;
	var->n = 0;
}

void	echo_helper(t_bvars var, t_cmds *tmp, int out_fd)
{
	while (var.i < var.n)
		var.i++;
	while (tmp->option[var.i])
	{
		write(1, tmp->option[var.i], ft_strlenn(tmp->option[var.i]));
		var.i++;
		if (tmp->option[var.i])
			write(1, " ", 1);
	}
	(dup2(out_fd, 1), close(out_fd));
}

void	echo_helper2(t_bvars var, t_cmds *tmp)
{
	while (var.i < var.n)
		var.i++;
	while (tmp->option[var.i])
	{
		write(1, tmp->option[var.i], ft_strlenn(tmp->option[var.i]));
		var.i++;
		if (tmp->option[var.i])
			write(1, " ", 1);
	}
	write(1, "\n", 1);
}

void	execute_echo(t_list *cmd)
{
	t_bvars	var;
	t_cmds	*tmp;
	int		out_fd;

	init_bvars(&var);
	tmp = (t_cmds *)cmd->content;
	out_fd = dup(1);
	if (tmp->files)
		(open_files(tmp), dup_fds(tmp));
	if (!tmp->option[0])
	{
		write(1, "\n", 1);
		(dup2(out_fd, 1), close(out_fd));
		return ;
	}
	while (tmp->option[var.j] && !check_echo_option(tmp->option[var.j], &var))
		var.j++;
	if (var.n)
	{
		echo_helper(var, tmp, out_fd);
		return ;
	}
	else
		echo_helper2(var, tmp);
	(dup2(out_fd, 1), close(out_fd));
}
