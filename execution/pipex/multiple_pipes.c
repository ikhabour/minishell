/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 15:40:24 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/28 19:00:04 by ikhabour         ###   ########.fr       */
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
		return (NULL);
	while (ptr->option[i])
		i++;
	argv = malloc(sizeof(char *) * (i + 1));
	argv[0] = ft_strdup(ptr->cmd_name);
	i = 0;
	while (ptr->option[j])
		argv[++i] = ft_strdup(ptr->option[j++]);
	argv[i] = NULL;
	return (argv);
}


// void	first_command(t_list *commands, char **enpv, int fdin, int fdout)
// {
// 	int pid;
// 	char **argv;
// 	char **paths;
// 	char *f_path;
// 	int i;
// 	(void)fdin;
// 	t_cmds *ptr;
// 	i = 0;

// 	ptr = (t_cmds *)commands->content;

// 	argv = make_argv(commands);
// 	pid = fork();
// 	if (pid == -1)
// 		return ;
// 	if (pid == 0)
// 	{
// 		dup2(fdout, 1);
// 		if (execute_builtins(commands, env))
// 			exit(0);
// 		if (access(ptr->cmd_name, X_OK) == 0)
// 			execve(ptr->cmd_name, argv, envp);
// 		paths = get_path(envp);
// 		if (!paths)
// 			msg_exit(ptr->cmd_name, ": No such file or directory\n", 127);
// 		while (paths[i])
// 		{
// 			f_path = paths[i];
// 			paths[i] = ft_strjoinn(f_path, ptr->cmd_name);
// 			free(f_path);
// 			if (access(paths[i], X_OK) == 0)
// 				break ;
// 			i++;
// 		}
// 		execve(paths[i], argv, envp);
// 		msg_exit(ptr->cmd_name, ": command not found\n", 127);
// 	}
// }

// void	multiple_pipes(t_list *commands, t_list **env)
// {
// 	int i;
// 	(void)env;
// 	i = 0;
// 	int pipes = count_pipes(commands);
// 	int **fd;
// 	int *pids;
// 	char **envp = env_to_array(env);
// 	pids = malloc(sizeof(int) * (pipes));
// 	fd = malloc(sizeof(int *) *(pipes));
// 	while (i < pipes)
// 	{
// 		fd[i] = malloc(sizeof(int) * 2);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < pipes)
// 	{
// 		if (pipe(fd[i]) < 0)
// 			return ;
// 		i++;
// 	}
// 	i = 0;
// 	int j = 0;
// 	while (i < pipes)
// 	{
// 		pids[i] = fork();
// 		if (pids[i] == -1)
// 			return ;
// 		if (pids[i] == 0)
// 		{
// 			if (i == 0)
// 				first_command(commands, envp, 0, fd[i][1]);
// 			else if (i == pipes - 1)
// 				dup2(fd[i - 1][0], 0);
// 			else
// 			{
// 				dup2(fd[i - 1][0], 0);
// 				dup2(fd[i][1], 1);
// 			}
// 		}
// 		while (j < pipes)
// 		{
// 			close(fd[i][0]);
// 			close(fd[i][1]);
// 			i++;
// 		}
// 		i++;
// 		// execve()
// 	}
// }