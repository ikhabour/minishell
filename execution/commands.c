/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 21:48:37 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/06 22:09:11 by ikhabour         ###   ########.fr       */
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

void	open_file_type(t_filetype *files)
{
	if (!ft_strcmp(files->type, "INPUT"))
		files->fd = open(files->file_name, O_RDONLY, 0644);
	else if (!ft_strcmp(files->type, "OUTPUT"))
		files->fd = open(files->file_name, O_CREAT | O_TRUNC | O_RDWR, 0644);
	else if (!ft_strcmp(files->type, "APPEND"))
		files->fd = open(files->file_name, O_CREAT | O_APPEND | O_RDWR, 0644);
	else
		files->fd = -1;
		
}

void	open_files(t_cmds *ptr)
{
	t_filetype *files;
	t_list *tmp;

	if (!ptr->files)
		return ;
	tmp = ptr->files;
	files = (t_filetype *)ptr->files->content;
	while (tmp)
	{
		open_file_type(files);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}

}

void	dup_fds(t_cmds *ptr)
{
	t_filetype *files;

	t_list *tmp;

	if (!ptr->files)
		return ;
	files = (t_filetype *)ptr->files->content;
	tmp = ptr->files;
	while (tmp)
	{
		if (!ft_strcmp(files->type, "OUTPUT") || !ft_strcmp(files->type, "APPEND"))
			dup2(files->fd, 1);
		else if(!ft_strcmp(files->type, "INPUT"))
			dup2(files->fd, 0);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
	
}

int	has_redirection(char **args)
{
	int i;

	i = 0;
	while (args[i])
	{
		if (args[i][0] == '<' || args[i][0] == '>')
			return (1);
		i++;
	}
	return (0);
}

int	execute_commands(t_list *cmd, t_list **env, char **args)
{
	char **envp;
	char **paths;
	char *f_path;
	int pid;
	int i;
	t_cmds *ptr;

	ptr = (t_cmds *)cmd->content;
	envp = env_to_array(env);
	pid = fork();
	if (has_redirection(args))
		args = make_argv(cmd);
	if (pid == -1)
		(write(2, "Fork Failed\n", 12), exit(1));
	else if (pid == 0)
	{
		open_files(ptr);
		dup_fds(ptr);
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
			printf("path : (%s)\n", paths[i]);
			free(f_path);
			if (access(paths[i], X_OK) == 0)
				break ;	
			i++;
		}
		execve(paths[i], args, envp);
		msg_exit(ptr->cmd_name, ": command not found\n", 127);
	}
	else
	{
		waitpid(pid, &i, 0);
		exit_s = WEXITSTATUS(i);
	}
	free_2d(envp);
	return (1);
}