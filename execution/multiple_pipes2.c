/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:21:17 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:42:55 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	first_command(t_list *commands, t_list **env, int *fd)
{
	t_spipes	var;
	int			i;
	t_cmds		*ptr;

	i = 0;
	ptr = (t_cmds *)commands->content;
	var.envp = env_to_array(env);
	var.argv = make_argv(commands);
	subpipes_helper(ptr, commands, env, fd);
	if (access(ptr->cmd_name, X_OK) == 0)
		execve(ptr->cmd_name, var.argv, var.envp);
	var.paths = get_path(var.envp);
	if (!var.paths)
		msg_exit(ptr->cmd_name, ": No such file or directory\n", 127);
	while (var.paths[i])
	{
		var.f_path = var.paths[i];
		var.paths[i] = ft_strjoinn(var.f_path, ptr->cmd_name);
		free(var.f_path);
		if (access(var.paths[i], X_OK) == 0)
			break ;
		i++;
	}
	execve(var.paths[i], var.argv, var.envp);
	msg_exit(ptr->cmd_name, ": command not found\n", 127);
}

void	last_command(t_list *commands, t_list **env, int *fd)
{
	t_spipes	var;
	int			i;
	t_cmds		*ptr;

	i = 0;
	ptr = (t_cmds *)commands->content;
	var.envp = env_to_array(env);
	var.argv = make_argv(commands);
	subpipes_helper2(ptr, commands, env, fd);
	if (access(ptr->cmd_name, X_OK) == 0)
		execve(ptr->cmd_name, var.argv, var.envp);
	var.paths = get_path(var.envp);
	if (!var.paths)
		msg_exit(ptr->cmd_name, ": No such file or directory\n", 127);
	while (var.paths[i])
	{
		var.f_path = var.paths[i];
		var.paths[i] = ft_strjoinn(var.f_path, ptr->cmd_name);
		free(var.f_path);
		if (access(var.paths[i], X_OK) == 0)
			break ;
		i++;
	}
	execve(var.paths[i], var.argv, var.envp);
	msg_exit(ptr->cmd_name, ": command not found\n", 127);
}

void	subpipes_helper3(t_cmds *ptr, int *fdout, int *fdin)
{
	if (!open_files(ptr))
		exit(1);
	if (output_file(ptr))
		dup_output_file(ptr);
	else
		dup2(fdout[1], 1);
	if (input_file(ptr))
		dup_input_file(ptr);
	else
		dup2(fdin[0], 0);
	close(fdout[0]);
	close(fdin[1]);
	if (!ptr->cmd_name)
		exit(0);
}

void	subpipes_helper4(t_cmds *ptr, t_list *commands, t_list **env,
		t_spipes var)
{
	if (execute_builtins(commands, env))
		exit(0);
	if (access(ptr->cmd_name, X_OK) == 0)
		execve(ptr->cmd_name, var.argv, var.envp);
}

void	middle_command(t_list *commands, t_list **env, int *fdin, int *fdout)
{
	t_spipes	var;
	int			i;
	t_cmds		*ptr;

	i = 0;
	ptr = (t_cmds *)commands->content;
	var.envp = env_to_array(env);
	var.argv = make_argv(commands);
	subpipes_helper3(ptr, fdout, fdin);
	subpipes_helper4(ptr, commands, env, var);
	var.paths = get_path(var.envp);
	if (!var.paths)
		msg_exit(ptr->cmd_name, ": No such file or directory\n", 127);
	while (var.paths[i])
	{
		var.f_path = var.paths[i];
		var.paths[i] = ft_strjoinn(var.f_path, ptr->cmd_name);
		free(var.f_path);
		if (access(var.paths[i], X_OK) == 0)
			break ;
		i++;
	}
	execve(var.paths[i], var.argv, var.envp);
	msg_exit(ptr->cmd_name, ": command not fosund\n", 127);
}
