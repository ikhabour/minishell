/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:31:49 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/13 20:12:19 by ikhabour         ###   ########.fr       */
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
	if (!tmp->option[0])
	{
		write(1, "\n", 1);
		dup2(out_fd, 1);
		close(out_fd);
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
			write(1, tmp->option[var.i], ft_strlenn(tmp->option[var.i]));
			var.i++;
			if (tmp->option[var.i])
				write(1, " ", 1);
		}
		dup2(out_fd, 1);
		close(out_fd);
		return ;
	}
	else
	{
		while (var.i < var.n)
			var.i++;
		while (tmp->option[var.i])
		{
			write(1, tmp->option[var.i], ft_strlenn(tmp->option[var.i]));
			var.i++;
			if (tmp->option[var.i])
				write(1, " ", 1);
		}
		write(1, "\n", 1);
	}
	dup2(out_fd, 1);
	close(out_fd);
}

t_list	*make_env(char **envp)
{
	t_list	*env;
	int		i;
	static int j;
	char *pwd;

	env = NULL;
	i = 0;
	if (!*envp)
	{
		pwd = getcwd(NULL, 0);
		pwd = ft_strjoinn("PWD=", pwd);
		ft_lstadd_backk(&env, ft_lstneww("OLDPWD"));
		ft_lstadd_backk(&env, ft_lstneww(pwd));
		ft_lstadd_backk(&env, ft_lstneww("SHLVL=0"));
		ft_lstadd_backk(&env, ft_lstneww("_=/usr/bin/env"));
		j++;
		return (env);
	}
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
	if (!ptr->option[0])
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

void	print_env(t_list **env)
{
	t_list *tmp;
	
	tmp = *env;
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
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

int	valid_identifier(char *str)
{
	int i;

	i = 0;
	if (!str[i])
		return (0);
	if (is_space(str))
		return (0);
	if (str[i] && ((str[i] >= 'a' && str[i] <= 'z') || str[i] == '_'))
		i++;
	while ((str[i] >= 'a' && str[i] <= 'z') || str[i] == '_')
		i++;
	if (!str[i])
		return (1);
	return (0);
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
	if (!ptr->option[0])
	{
		(print_export(env), dup2(fd, 1));
		return ;
	}
	while (ptr->option[var.j])
	{
		if (!valid_identifier(ptr->option[var.j]))
		{
			write(2, "Minishell : export: `", 21);
			write(2, ptr->option[var.j], ft_strlenn(ptr->option[var.j]));
			write(2, "': not a valid identifier\n", 26);
			var.j++;
			continue ;
		}
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
			{
				ft_lstadd_backk(env, ft_lstneww(ft_strdup(ptr->option[var.j])));
			}
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
	// if (!valid_identifier(ptr->option))
	// {
	// 	write(2, ": not a valid identifier\n", 25);
	// 	exit_s = 1;
	// 	return ;
	// }
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

void	execute_exit(t_list *cmd, t_list **env)
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
	if (!ptr->option[0])
	{
		exit(0);
		shlvl_edit(env, 1);
	}
	while (ptr->option[i])
		i++;
	if (i > 1)
	{
		(write(2, "exit: too many arguments\n", 25), dup2(fd, 1));
		return_val = 1;
		return ;
	}
	if (!is_digit(ptr->option[0]))
	{
		write(2, "exit: ", 6);
		shlvl_edit(env, 1);
		msg_exit(ptr->option[0], ": numeric argument required\n", 255);
	}
	exit_status = ft_atoi(ptr->option[0]);
	if (exit_status < 0)
	{
		exit_status += 255;
		shlvl_edit(env, 1);
		exit(exit_status);
	}
	else if (exit_status > 255)
	{
		exit_status -= 256;
		shlvl_edit(env, 1);
		exit(exit_status);
	}
	else
	{
		shlvl_edit(env, 1);
		exit(exit_status);
	}
	dup2(fd, 1);
}

void	exportt(char *value, char *var, t_list **env)
{
	t_list *tmp;
	static int i;

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

void	setpwd(t_list **env)
{
	char *pwd;
	t_list *tmp;
	static int i;

	pwd = getcwd(NULL, 0);
	tmp = *env;
	while (tmp && ft_strncmpp(tmp->content, "PWD=", 4))
		tmp = tmp->next;
	if (i > 0)
		free(tmp->content);
	tmp->content = ft_strjoinn("PWD=", pwd);
	free(pwd);
	i++;
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
	if (!ptr->option[0])
	{
		while (tmp && ft_strncmpp(tmp->content, "HOME=", 5))
			tmp = tmp->next;
		if (!tmp)
		{
			(write(2, "cd: HOME not set\n", 17), dup2(fd, 1));
			close(fd);
			return_val = 1;
			return ;
		}
		(chdir(tmp->content + 5), dup2(fd, 1));
		close(fd);
		setpwd(env);
		return ;
	}
	if (access(ptr->option[0], F_OK))
	{
		(write(2, "Minishell: cd: No such file or directory\n", 41), dup2(fd, 1));
		close(fd);
		return ;
	}
	else if (access(ptr->option[0], X_OK))
	{
		(write(2, "Minishell: cd: Permission Denied!\n", 21), dup2(fd, 1));
		close(fd);
		return ;
	}
	(chdir(ptr->option[0]), dup2(fd, 1));
	setpwd(env);
	close(fd);
}

void	return_val_close(int fd)
{
	return_val = 0;
	close(fd);
}

int	execute_builtins(t_list *cmd, t_list **env)
{
	t_cmds	*tmp;
	int fd;

	tmp = (t_cmds *)cmd->content;
	fd = dup(1);
	if (!tmp->cmd_name)
	{	
		close(fd);
		return (0);
	}
	if (!ft_strcmpp(tmp->cmd_name, "pwd"))
	{
		if (tmp->files)
		{
			open_files(tmp);
			dup_fds(tmp);
		}
		free(tmp->cmd_name);
		tmp->cmd_name = getcwd(NULL, 0);
		(printf("%s\n", tmp->cmd_name), dup2(fd, 1));
		return_val_close(fd);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "echo"))
	{
		execute_echo(cmd);
		return_val_close(fd);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "cd"))
	{
		execute_cd(cmd, env);
		close(fd);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "env"))
	{
		execute_env(env, cmd);
		return_val_close(fd);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "export"))
	{
		execute_export(cmd, env);
		return_val_close(fd);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "unset"))
	{
		execute_unset(cmd, env);
		return_val_close(fd);
		return (1);
	}
	else if (!ft_strcmpp(tmp->cmd_name, "exit"))
	{
		execute_exit(cmd, env);
		return (1);
	}
	close(fd);
	return (0);
}