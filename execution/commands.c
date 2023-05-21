/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 21:48:37 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/21 15:12:13 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_len(t_list **env)
{
	int i;
	t_list *tmp;

	i = 0;
	tmp = *env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);

}

char	**env_to_array(t_list **env)
{
	t_list *tmp;
	int i;
	char **array;

	tmp = *env;
	i = count_len(env);
	array = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (tmp)
	{
		array[i++] = ft_strdupp(tmp->content);
		tmp = tmp->next;
	}
	array[i] = NULL;
	return (array);

}

char **get_path(char **envp)
{
	int i;
	char **paths;
	char *save;

	i = 0;
	while (envp[i] && ft_strncmpp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return NULL;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		save = paths[i];
		paths[i] = ft_strjoinn(paths[i], "/");
		free(save);
		i++;
	}
	return (paths);
	
}

int get_len(char **arg)
{
	int i;

	i = 0;
	while (arg[i])
		i++;
	return (i);
}

void	msg_exit(char *msg, char *msg1, int status)
{
	write(2, msg, ft_strlenn(msg));
	write(2, msg1, ft_strlenn(msg1));
	exit(status);
}

void	execute_commands(t_list *cmd, t_list **env, char **args)
{
	char **envp;
	char **paths;
	char *f_path;
	int pid;
	int exec;
	int i;
	t_cmds *ptr;

	ptr = (t_cmds *)cmd->content;
	envp = env_to_array(env);
	pid = fork();
	if (pid == -1)
		(write(2, "Fork Failed\n", 12), exit(1));
	else if (pid == 0)
	{
		if (access(ptr->cmd_name, X_OK) == 0)
			execve(ptr->cmd_name, args, envp);
		paths = get_path(envp);
		i = 0;
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
		exec = execve(paths[i], args, envp);
		if (exec == -1)
		{
			printf("Minishell: %s: command not found\n", ptr->cmd_name);
			exit(127);
		}
	}
	else
		waitpid(pid, &i, 0);
}