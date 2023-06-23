/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:31:49 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:14:08 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_builtins_pt1(t_list *cmd, t_cmds *tmp, t_list **env, int fd)
{
	if (!ft_strcmpp(tmp->cmd_name, "env"))
	{
		(execute_env(env, cmd), close(fd));
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "export"))
	{
		(execute_export(cmd, env), close(fd));
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "unset"))
	{
		(execute_unset(cmd, env), close(fd));
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "exit"))
	{
		execute_exit(cmd, env);
		return (1);
	}
	return (0);
}

int	exec_builtins_pt2(t_list *cmd, t_cmds *tmp, t_list **env, int fd)
{
	(void)env;
	if (!ft_strcmpp(tmp->cmd_name, "pwd"))
	{
		if (tmp->files)
			(open_files(tmp), dup_fds(tmp));
		g_sigs.exit_s = 0;
		free(tmp->cmd_name);
		tmp->cmd_name = getcwd(NULL, 0);
		if (!tmp->cmd_name)
		{
			write(2, "bash : PATH NOT FOUND\n", 22);
			return (1);
		}
		(printf("%s\n", tmp->cmd_name), dup2(fd, 1));
		close(fd);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "echo"))
	{
		(execute_echo(cmd), close(fd), g_sigs.exit_s = 0);
		return (1);
	}
	return (0);
}

int	execute_builtins(t_list *cmd, t_list **env)
{
	t_cmds	*tmp;
	int		fd;
	int		return_val;

	tmp = (t_cmds *)cmd->content;
	fd = dup(1);
	if (!tmp->cmd_name)
	{
		close(fd);
		return (0);
	}
	return_val = exec_builtins_pt1(cmd, tmp, env, fd);
	if (return_val)
		return (return_val);
	return_val = exec_builtins_pt2(cmd, tmp, env, fd);
	if (return_val)
		return (return_val);
	else if (!ft_strcmpp(tmp->cmd_name, "cd"))
	{
		(execute_cd(cmd, env), close(fd));
		return (1);
	}
	close(fd);
	return (0);
}
