/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 21:48:37 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:16:25 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cmd_checks(t_cmds *ptr)
{
	if (!open_files(ptr))
		exit(1);
	dup_fds(ptr);
	if (!ptr->cmd_name)
		exit(0);
	if (ptr->cmd_name[0] == '.' && access(ptr->cmd_name, X_OK))
		msg_exit(ptr->cmd_name, ": No such file or directory\n", 127);
}

void	cmds_while(t_exec *vars, char **paths, t_cmds *ptr)
{
	while (paths[vars->i])
	{
		vars->f_path = paths[vars->i];
		paths[vars->i] = ft_strjoinn(vars->f_path, ptr->cmd_name);
		free(vars->f_path);
		if (access(paths[vars->i], X_OK) == 0)
			break ;
		vars->i++;
	}
}

void	cmds_free(char **envp, char **args)
{
	free_2d(envp);
	if (args)
		free_2d(args);
}

void	cmd_helper(t_cmds *ptr, char **args, char **envp, t_exec vars)
{
	char	**paths;

	cmd_checks(ptr);
	(access(ptr->cmd_name, X_OK) == 0) && execve(ptr->cmd_name, args, envp);
	paths = get_path(envp);
	vars.i = 0;
	if (!paths)
		msg_exit(ptr->cmd_name, ": No such file or directory\n", 127);
	cmds_while(&vars, paths, ptr);
	execve(paths[vars.i], args, envp);
	msg_exit(ptr->cmd_name, ": command not found\n", 127);
}

int	execute_commands(t_list *cmd, t_list **env, char **args)
{
	char	**envp;
	t_exec	vars;
	t_cmds	*ptr;

	ptr = (t_cmds *)cmd->content;
	envp = env_to_array(env);
	vars.pid = fork();
	g_sigs.process = 1;
	if (has_redirection(args))
	{
		free_2d(args);
		args = make_argv(cmd);
	}
	if (vars.pid == -1)
		(write(2, "Fork Failed\n", 12), exit(1));
	else if (vars.pid == 0)
		cmd_helper(ptr, args, envp, vars);
	else
		wait_process(&vars);
	return (cmds_free(envp, args), 1);
}
