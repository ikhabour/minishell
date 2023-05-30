/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 15:40:24 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/30 16:36:04 by ikhabour         ###   ########.fr       */
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

char	**make_argv(t_list *commands)
{
	char **argv;
	t_cmds *ptr;
	int i;
	int j;

	ptr = (t_cmds *)commands->content;
	i = 0;
	j = 0;
	if (!ptr->option)
	{
		argv = malloc(sizeof(char *) * 2);
		argv[0] = ptr->cmd_name;
		argv[1] = NULL;
		return (argv);
	}
	while (ptr->option[i])
		i++;
	argv = malloc(sizeof(char *) * (i + 2));
	argv[0] = ft_strdup(ptr->cmd_name);
	i = 1;
	while (ptr->option[j])
		argv[i++] = ft_strdup(ptr->option[j++]);
	argv[i] = NULL;
	return (argv);
}

void	first_command(t_list *commands, t_list **env, int fd)
{
	char **argv;
	char **paths;
	char *f_path;
	char **envp;
	int i;
	t_cmds *ptr;
	i = 0;
	ptr = (t_cmds *)commands->content;
	envp = env_to_array(env);
	argv = make_argv(commands);
	dup2(fd, 1);
	if (execute_builtins(commands, env))
		exit(0);
	if (access(ptr->cmd_name, X_OK) == 0)
		execve(ptr->cmd_name, argv, envp);
	paths = get_path(envp);
	if (!paths)
		msg_exit(ptr->cmd_name, ": No such file or directory\n", 127);
	while (paths[i])
	{
		f_path = paths[i];
		paths[i] = ft_strjoinn(f_path, ptr->cmd_name);
		free(f_path);
		if (access(paths[i], X_OK) == 0)
			break ;
		i++;
	}
	close(fd);
	execve(paths[i], argv, envp);
	msg_exit(ptr->cmd_name, ": command not found\n", 127);
}

void	last_command(t_list *commands, t_list **env, int fd)
{
	char **argv;
	char **paths;
	char *f_path;
	char **envp;
	int i;
	t_cmds *ptr;
	i = 0;

	ptr = (t_cmds *)commands->content;
	envp = env_to_array(env);
	argv = make_argv(commands);
	dup2(fd, 0);
	if (execute_builtins(commands, env))
		exit(0);
	if (access(ptr->cmd_name, X_OK) == 0)
		execve(ptr->cmd_name, argv, envp);
	paths = get_path(envp);
	if (!paths)
		msg_exit(ptr->cmd_name, ": No such file or directory\n", 127);
	while (paths[i])
	{
		f_path = paths[i];
		paths[i] = ft_strjoinn(f_path, ptr->cmd_name);
		free(f_path);
		if (access(paths[i], X_OK) == 0)
			break ;
		i++;
	}
	close(fd);
	execve(paths[i], argv, envp);
	msg_exit(ptr->cmd_name, ": command not found\n", 127);
}
	
void	middle_command(t_list *commands, t_list **env, int fdin, int fdout)
{
	char **argv;
	char **paths;
	char *f_path;
	char **envp;
	int i;
	t_cmds *ptr;
	i = 0;

	ptr = (t_cmds *)commands->content;
	envp = env_to_array(env);
	argv = make_argv(commands);	
	dup2(fdout, 1);
	dup2(fdin, 0);
	if (execute_builtins(commands, env))
		exit(0);
	if (access(ptr->cmd_name, X_OK) == 0)
		execve(ptr->cmd_name, argv, envp);
	paths = get_path(envp);
	if (!paths)
		msg_exit(ptr->cmd_name, ": No such file or directory\n", 127);
	while (paths[i])
	{
		f_path = paths[i];
		paths[i] = ft_strjoinn(f_path, ptr->cmd_name);
		free(f_path);
		if (access(paths[i], X_OK) == 0)
			break ;
		i++;
	}
	execve(paths[i], argv, envp);
	msg_exit(ptr->cmd_name, ": command not found\n", 127);
}

void	multiple_pipes(t_list *commands, t_list **env)
{
	int i;
	i = 0;
	int pipes = count_pipes(commands);
	int **fd;
	int *pids;
	// int status = 0;

	fd = malloc(sizeof(int *) *(pipes + 1));
	pids = malloc(sizeof(int) * (pipes + 1));
	while (i < pipes)
	{
		fd[i] = malloc(sizeof(int) * 2);
		i++;
	}
	fd[i] = NULL;
	i = 0;
	int j;
	while (i < pipes + 1)
	{
		if (i < pipes && pipe(fd[i]) < 0)
	 		return ;
		pids[i] = fork();
		if (pids[i] == 0)
		{
			if (i == 0)
			{
				first_command(commands, env, fd[i][1]);
				close(fd[i][0]);
			}
			else if (i == pipes)
			{
				last_command(commands, env, fd[i - 1][0]);
				close(fd[i - 1][1]);
			}
			else
			{
				middle_command(commands, env, fd[i - 1][0], fd[i][1]);
				close(fd[i - 1][1]);
				close(fd[i][0]);
			}
		}
		if (i != 0)
		{
			close(fd[i - 1][0]);
			close(fd[i - 1][1]);
		}
		i++;
		commands = commands->next;
	}
	j = 0;
	while (j < pipes)
	{
		close(fd[j][0]);
		close(fd[j][1]);
		waitpid(pids[j], &status, 0);
		j++;
	}
	waitpid(pids[j], &status, 0);
}