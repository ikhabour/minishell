/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:13:45 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:13:55 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	write_and_exit(int exit_status, t_list **env)
{
	write_exit();
	shlvl_edit(env, 1);
	exit(exit_status);
}

void	check_exit_status(int exit_status, t_list **env)
{
	if (exit_status < 0)
	{
		exit_status += 255;
		write_and_exit(exit_status, env);
	}
	else if (exit_status > 255)
	{
		exit_status -= 256;
		write_and_exit(exit_status, env);
	}
	else
		write_and_exit(exit_status, env);
}

void	check_exit_errors(t_cmds *ptr, t_list **env)
{
	if (!ptr->option[0])
	{
		write_exit();
		shlvl_edit(env, 1);
		exit(0);
	}
	if (!is_digit(ptr->option[0]))
	{
		write_exit();
		shlvl_edit(env, 1);
		msg_exit(ptr->option[0], ": numeric argument required\n", 255);
	}
}

void	execute_exit(t_list *cmd, t_list **env)
{
	t_cmds	*ptr;
	int		exit_status;
	int		i;
	int		fd;

	i = 0;
	ptr = (t_cmds *)cmd->content;
	fd = dup(1);
	if (ptr->files)
		(open_files(ptr), dup_fds(ptr));
	check_exit_errors(ptr, env);
	while (ptr->option[i])
		i++;
	if (i > 1)
	{
		write(2, "exit\n", 6);
		(write(2, "exit: too many arguments\n", 25), dup2(fd, 1));
		g_sigs.exit_s = 1;
		return ;
	}
	exit_status = ft_atoi(ptr->option[0]);
	(check_exit_status(exit_status, env), dup2(fd, 1));
}

void	exportt(char *value, char *var, t_list **env)
{
	t_list		*tmp;
	static int	i;

	tmp = *env;
	while (tmp && ft_strncmpp(tmp->content, var, ft_strlenn(var)))
		tmp = tmp->next;
	if (!tmp)
		return ;
	if (i > 0)
		free(tmp->content);
	tmp->content = ft_strjoinn(var, value);
	free(value);
	i++;
}
