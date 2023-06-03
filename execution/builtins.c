/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:31:49 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/03 23:17:14 by ikhabour         ###   ########.fr       */
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

void	close_fds(t_cmds *ptr)
{
	t_filetype *files;
	t_list *tmp;

	tmp = ptr->files;
	files = (t_filetype *)tmp->content;
	while (tmp)
	{
		close(files->fd);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
}

void	execute_echo(t_list *cmd)
{
	t_bvars	var;
	t_cmds	*tmp;
	int	out_fd;

	var.n = 0;
	var.j = 0;
	tmp = (t_cmds *)cmd->content;
	var.i = 0;
	out_fd = dup(1);
	if (tmp->files)
	{
		open_files(tmp);
		dup_fds(tmp);
	}
	if (!tmp->option)
	{
		printf("\n");
		dup2(out_fd, 1);
		return ;
	}
	while (tmp->option[var.j] && !check_echo_option(tmp->option[var.j], &var))
		var.j++;
	if (var.n)
	{
		while (var.i < var.n)
			var.i++;
		while (tmp->option[var.i])
		{
			printf("%s", tmp->option[var.i++]);
			if (tmp->option[var.i])
				printf(" ");
		}
		dup2(out_fd, 1);
		return ;
	}
	else
	{
		while (var.i < var.n)
			var.i++;
		while (tmp->option[var.i])
		{
			printf("%s", tmp->option[var.i++]);
			if (tmp->option[var.i])
				printf(" ");
		}
		printf("\n");
	}
	dup2(out_fd, 1);
}

t_list	*make_env(char **envp)
{
	t_list	*env;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
		ft_lstadd_backk(&env, ft_lstneww(envp[i++]));
	return (env);
}

int	change_value(char *argument)
{
	int	i;

	i = 0;
	while (argument[i] && argument[i] != '=')
		i++;
	if (i == ft_strlenn(argument))
		return (0);
	return (1);
}

void	execute_env(t_list **env, t_list *cmd)
{
	t_list	*tmp;
	t_cmds *ptr;
	int fd;

	tmp = *env;
	ptr = (t_cmds *)cmd->content;
	fd = dup(1);
	if (ptr->files)
	{
		open_files(ptr);
		dup_fds(ptr);
	}
	if (!ptr->option)
	{
		while (tmp)
		{
			if (change_value(tmp->content))
				printf("%s\n", tmp->content);
			tmp = tmp->next;
		}
		dup2(fd, 1);
		return ;
	}
	(printf("env: %s: No such file or directory\n", ptr->option[0]), dup2(fd, 1));
}

int	append_value(char *argument)
{
	int	i;

	i = 0;
	while (argument[i] && argument[i] != '+')
		i++;
	if (argument[i] && argument[i + 1] == '=')
		return (1);
	return (0);
}

char	*remove_plus(char *string)
{
	char	*s;
	int		i;
	int		j;

	s = malloc(ft_strlenn(string) * sizeof(char));
	i = 0;
	j = 0;
	while (string[i])
	{
		if (string[i] == '+')
			i++;
		s[j++] = string[i];
		i++;
	}
	s[j] = '\0';
	return (s);
}

void	print_string(char *str)
{
	int i;

	i = 0;
	write(1, "declare -x ", 11);
	if (change_value(str))
	{
		while (str[i] && str[i] != '=')
		{
			write(1, &str[i], 1);
			i++;
		}
		write(1, &str[i], 1);
		write(1, "\"", 1);
		i++;
		while (str[i])
		{
			write(1, &str[i], 1);
			i++;
		}
		write(1, "\"", 1);
	}
	else
		printf("%s", str); 
}

void	print_export(t_list **env)
{
	t_list *tmp;

	tmp = *env;
	while (tmp)
	{
		print_string(tmp->content);
		printf("\n");
		tmp = tmp->next;
	}
}

int	var_exists(char *var, t_list **env)
{
	t_list *tmp;

	tmp = *env;
	while (ft_strncmp(tmp->content, var, ft_strlenn(var)))
		tmp = tmp->next;
	if (!tmp)
		return (0);
	return (1);
}

void	execute_export(t_list *cmd, t_list **env)
{
	t_cmds	*ptr;
	t_bvars var;
	t_list	*tmp;
	char *temp;
	int fd;

	ptr = (t_cmds *)cmd->content;
	var.i = 0;
	var.j = 0;
	var.n = 0;
	tmp = *env;
	fd = dup(1);

	if (ptr->files)
	{
		open_files(ptr);
		dup_fds(ptr);
	}
	if (!ptr->option)
	{
		(print_export(env), dup2(fd, 1));
		return ;
	}
	while (ptr->option[var.j])
	{
		if (append_value(ptr->option[var.j]))
		{
			var.i = 0;
			tmp = *env;
			while (ptr->option[var.j][var.i] != '=')
				var.i++;
			while (tmp && ft_strncmpp(tmp->content, ptr->option[var.j], var.i - 1))
				tmp = tmp->next;
			if (!tmp)
			{
				ft_lstadd_backk(env, ft_lstneww(remove_plus(ptr->option[var.j])));
			}
			else
			{
				if (!change_value(tmp->content))
				{
					temp = tmp->content;
					tmp->content = ft_strjoinn(temp, ptr->option[var.j] + (var.i));
					free(temp);
				}
				else
				{
					temp = tmp->content;
					tmp->content = ft_strjoinn(temp, ptr->option[var.j] + (var.i + 1));
					free(temp);
				}
			} 
		}
		else if (change_value(ptr->option[var.j]))
		{
			var.i = 0;
			tmp = *env;
			while (ptr->option[var.j][var.i] != '=')
				var.i++;
			while (tmp && ft_strncmpp(tmp->content, ptr->option[var.j], var.i))
				tmp = tmp->next;
			if (!tmp)
			{
				ft_lstadd_backk(env, ft_lstneww(ptr->option[var.j]));
				continue ;
			}
			tmp->content = ft_strdup(ptr->option[var.j]);
		}
		else
		{
			var.i = 0;
			while (ptr->option[var.j][var.i])
				var.i++;
			tmp = *env;
			while (tmp && ft_strncmpp(tmp->content, ptr->option[var.j], var.i))
				tmp = tmp->next;
			if (!tmp)
				ft_lstadd_backk(env, ft_lstneww(ptr->option[var.j]));
		}
		var.j++;
	}
	dup2(fd, 1);
}

void	execute_unset(t_list *cmd, t_list **env)
{
	t_list	*curr;
	t_list	*prev;
	t_cmds	*ptr;
	int fd;
	int i;

	ptr = (t_cmds *)cmd->content;
	i = 0;
	fd = dup(1);
	if (ptr->files)
	{
		open_files(ptr);
		dup_fds(ptr);
	}
	if (!ptr->option)
	{
		(write(2,"unset: not enough arguments\n", 28), dup2(fd, 1));
		return ;
	}
	while (ptr->option[i])
	{
		curr = *env;
		prev = NULL;
		if (curr && !ft_strncmpp(curr->content, ptr->option[i],
				ft_strlenn(ptr->option[i])))
		{
			*env = curr->next;
			(free(curr), dup2(fd, 1));
			return ;
		}
		while (curr && ft_strncmpp(curr->content, ptr->option[i],
				ft_strlenn(ptr->option[i])))
		{
			prev = curr;
			curr = curr->next;
		}
		if (curr == NULL)
		{
			dup2(fd, 1);
			return ;
		}
		prev->next = curr->next;
		free(curr);
		i++;
	}
	dup2(fd, 1);
}

void	execute_exit(t_list *cmd)
{
	t_cmds	*ptr;
	int		exit_status;
	int i;
	int fd;

	i = 0;
	ptr = (t_cmds *)cmd->content;
	fd = dup(1);
	if (ptr->files)
	{
		open_files(ptr);
		dup_fds(ptr);
	}
	if (!ptr->option)
		exit(0);
	while (ptr->option[i])
		i++;
	if (i > 0)
	{
		(write(2, "exit: too many arguments\n", 25), dup2(fd, 1));
		return ;
	}
	if (!is_digit(ptr->option[0]))
	{
		write(2, "exit: ", 6);
		msg_exit(ptr->option[0], ": numeric argument required\n", 255);
	}
	exit_status = ft_atoi(ptr->option[0]);
	if (exit_status < 0)
	{
		exit_status += 255;
		exit(exit_status);
	}
	else if (exit_status > 255)
	{
		exit_status -= 256;
		exit(exit_status);
	}
	else
		exit(exit_status);
	dup2(fd, 1);
}

void	exportt(char *value, char *var, t_list **env)
{
	t_cmds ptr;
	t_list cmd;
	char *option[2];
	// t_filetype *pnt;
	int i;

	i = 0;
	// printf("segfault here\n");
	// pnt = (t_filetype *)ptr.files->content;
	option[0] = ft_strjoinn(var, value);
	option[1] = NULL;
	ptr.cmd_name = "export";
	ptr.option = option;
	// pnt->file_name = NULL;
	// pnt->type = NULL;
	// pnt->red = NULL;
	// pnt->fd = -1;
	cmd.content = &ptr;
	execute_export(&cmd, env);
	free(option[0]);
}

void	execute_cd(t_list *cmd, t_list **env)
{
	t_cmds *ptr;
	t_list *tmp; 
	char *dir;
	int fd;

	ptr = (t_cmds *)cmd->content;
	tmp = *env;
	fd = dup(1);
	dir = getcwd(NULL, 0);
	exportt(dir, "OLDPWD=", env);
	if (ptr->files)
	{
		open_files(ptr);
		dup_fds(ptr);
	}
	if (!ptr->option)
	{
		while (tmp && ft_strncmpp(tmp->content, "HOME=", 5))
			tmp = tmp->next;
		if (!tmp)
		{
			(write(2, "cd: HOME not set\n", 17), dup2(fd, 1));
			return ;
		}
		(chdir(tmp->content + 5), dup2(fd, 1));
		return ;
	}
	if (access(ptr->option[0], F_OK))
	{
		(write(2, "Minishell: cd: No such file or directory\n", 41), dup2(fd, 1));
		return ;
	}
	else if (access(ptr->option[0], X_OK))
	{
		(write(2, "Minishell: cd: Permission Denied!\n", 21), dup2(fd, 1));
		return ;
	}
	(chdir(ptr->option[0]), dup2(fd, 1));
}

int	execute_builtins(t_list *cmd, t_list **env)
{
	t_cmds	*tmp;
	int fd;

	tmp = (t_cmds *)cmd->content;
	fd = dup(1);
	if (!ft_strcmpp(tmp->cmd_name, "pwd"))
	{
		if (tmp->files)
		{
			open_files(tmp);
			dup_fds(tmp);
		}
		tmp->cmd_name = getcwd(NULL, 0);
		(printf("%s\n", tmp->cmd_name), dup2(fd, 1));
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "echo"))
	{
		execute_echo(cmd);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "cd"))
	{
		execute_cd(cmd, env);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "env"))
	{
		execute_env(env, cmd);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "export"))
	{
		execute_export(cmd, env);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "unset"))
	{
		execute_unset(cmd, env);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "exit"))
	{
		execute_exit(cmd);
		return (1);
	}
	return (0);
}