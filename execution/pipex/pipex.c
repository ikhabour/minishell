/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:41:55 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/25 15:35:18 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_list *array_to_list(char **envp)
{
	t_list *env;
	int i;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		ft_lstadd_backk(&env, ft_lstneww(envp[i]));
		i++;
	}
	return (env);
}

int	count_size(t_list *commands)
{
	int size = 1;
	int i = 0;
	t_cmds *ptr;

	ptr = (t_cmds *)commands->content;
	if (!ptr->option)
		return (size);
	else
	{
		while (ptr->option[i])
		{
			i++;
			size++;
		}
	}
	return (size);
}

char	**change_args(t_list *commands)
{
	t_cmds *ptr;
	int i = 0;
	char **new_args;
	int j = 1;

	ptr = (t_cmds *)commands->content;
	i = count_size(commands);
	new_args = malloc(sizeof(char *) * (i + 1));
	new_args[i] = NULL;
	new_args[0] = ptr->cmd_name;
	if (i == 1)
		return (new_args);
	i = 0;
	while (ptr->option[i])
	{
		new_args[j++] = ptr->option[i++];
	}
	return (new_args);
}


void	exec_cmd1(t_list *commands, int *fd, char **envp, char **argv)
{
	t_cmds *ptr;
	int		exec;
	t_list *env;
	int i;

	i = 0;

	argv = change_args(commands);
	env = array_to_list(envp);
	ptr = (t_cmds *)commands->content;
	dup2(fd[1], 1);
	close(fd[0]);
	if (execute_builtins(commands, &env))
		exit(0);
	if (access(ptr->cmd_name, X_OK) == 0)
		execve(ptr->cmd_name, argv, envp);
	exec = execve(bring_path(ptr->cmd_name, envp), argv, envp);
	if (exec == -1)
		msg_exit(ptr->cmd_name, ": command not found\n", 127);
}

void	exec_cmd2(t_list *commands, int *fd, char **envp, char **argv)
{
	t_cmds *ptr;
	int		exec;
	t_list *env;
	int i;

	i = 0;
	argv = change_args(commands);
	env = array_to_list(envp);
	ptr = (t_cmds *)commands->content;
	dup2(fd[0], 0);
	close(fd[1]);
	if (execute_builtins(commands, &env))
		exit(0);
	if (access(ptr->cmd_name, X_OK) == 0)
		execve(ptr->cmd_name, argv, envp);
	exec = execve(bring_path(ptr->cmd_name, envp), argv, envp);
	if (exec == -1)
		msg_exit(ptr->cmd_name, ": command not found\n", 127);
}

void	execute_pipe_commands(t_list *commands, char **envp, char **argv)
{
	int	fd[2];
	int	pid[2];

	if (pipe(fd) == -1)
		perror("Pipe failed!");
	pid[0] = fork();
	if (pid[0] == -1)
		perror("Fork failed!");
	if (pid[0] == 0)
		exec_cmd1(commands, fd, envp, argv);
	else
	{
		commands = commands->next;
		pid[1] = fork();
		if (pid[1] == -1)
			perror("Fork failed!");
		if (pid[1] == 0)
			exec_cmd2(commands, fd, envp, argv);
		else
			ft_wait(pid[0], fd, pid[1]);
	}
}
