/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:31:49 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/22 01:52:37 by ikhabour         ###   ########.fr       */
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

void	init_bvars(t_bvars *var)
{
	var->i = 0;
	var->j = 0;
	var->n = 0;
}

void	echo_helper(t_bvars var, t_cmds *tmp, int out_fd)
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
	(dup2(out_fd, 1), close(out_fd));
}

void	echo_helper2(t_bvars var, t_cmds *tmp)
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

void	execute_echo(t_list *cmd)
{
	t_bvars	var;
	t_cmds	*tmp;
	int		out_fd;

	init_bvars(&var);
	tmp = (t_cmds *)cmd->content;
	out_fd = dup(1);
	if (tmp->files)
		(open_files(tmp), dup_fds(tmp));
	if (!tmp->option[0])
	{
		write(1, "\n", 1);
		(dup2(out_fd, 1), close(out_fd));
		return ;
	}
	while (tmp->option[var.j] && !check_echo_option(tmp->option[var.j], &var))
		var.j++;
	if (var.n)
	{
		echo_helper(var, tmp, out_fd);
		return ;
	}
	else
		echo_helper2(var, tmp);
	(dup2(out_fd, 1), close(out_fd));
}

t_list	*make_env(char **envp)
{
	t_list		*env;
	int			i;
	static int	j;
	char		*pwd;

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
	t_cmds	*ptr;
	int		fd;

	tmp = *env;
	ptr = (t_cmds *)cmd->content;
	fd = dup(1);
	if (ptr->files)
		(open_files(ptr), dup_fds(ptr));
	if (!ptr->option[0])
	{
		while (tmp)
		{
			if (change_value(tmp->content))
				printf("%s\n", tmp->content);
			tmp = tmp->next;
		}
		(dup2(fd, 1), close(fd), sigs.exit_s = 0);
		return ;
	}
	(printf("env: %s: No such file or directory\n", ptr->option[0]), dup2(fd,
			1));
	close(fd);
	sigs.exit_s = 127;
}

int	append_value(char *argument)
{
	int	i;

	i = 0;
	while (argument[i])
	{
		if (argument[i] == '+' || argument[i] == '=')
			break ;
		i++;
	}
	if (argument[i] && argument[i] == '+' && argument[i + 1] == '=')
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
	int	i;

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
	t_list	*tmp;

	tmp = *env;
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
}

void	print_export(t_list **env)
{
	t_list	*tmp;

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
	t_list	*tmp;

	tmp = *env;
	while (ft_strncmp(tmp->content, var, ft_strlenn(var)))
		tmp = tmp->next;
	if (!tmp)
		return (0);
	return (1);
}

int	is_alpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

int	unset_valid(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (0);
	while (is_alpha(str[i]) || str[i] == '_')
		i++;
	if (!str[i])
		return (1);
	return (0);
}

int	valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (0);
	if (is_space(str))
		return (0);
	while (str[i])
	{
		if (str[i] == '+' || str[i] == '=')
			break ;
		i++;
	}
	if (i == 0)
		return (0);
	while (i >= 0)
	{
		if (str[i] && !is_alpha(str[i]) && str[i] != '_' && str[i] != '='
			&& str[i] != '+')
			return (0);
		i--;
	}
	return (1);
}

void	identifier_error(char *option)
{
	write(2, "Minishell : unset: `", 20);
	write(2, option, ft_strlenn(option));
	write(2, "': not a valid identifier\n", 26);
	sigs.exit_s = 1;
}

void	export_helper(t_bvars var, t_cmds *ptr, t_list *tmp, t_list **env)
{
	var.i = 0;
	while (ptr->option[var.j][var.i])
		var.i++;
	tmp = *env;
	while (tmp && ft_strncmpp(tmp->content, ptr->option[var.j], var.i))
		tmp = tmp->next;
	if (!tmp)
		ft_lstadd_backk(env, ft_lstneww(ft_strdup(ptr->option[var.j])));
}

void	export_helper2(t_bvars var, t_list *tmp, t_cmds *ptr, t_list **env)
{
	char	*temp;

	var.i = 0;
	tmp = *env;
	while (ptr->option[var.j][var.i] != '=')
		var.i++;
	while (tmp && ft_strncmpp(tmp->content, ptr->option[var.j], var.i - 1))
		tmp = tmp->next;
	if (!tmp)
		ft_lstadd_backk(env, ft_lstneww(remove_plus(ptr->option[var.j])));
	else
	{
		temp = tmp->content;
		if (!change_value(tmp->content))
		{
			tmp->content = ft_strjoinn(temp, ptr->option[var.j] + (var.i));
			free(temp);
		}
		else
		{
			tmp->content = ft_strjoinn(temp, ptr->option[var.j] + (var.i + 1));
			free(temp);
		}
	}
}

void	export_helper3(t_bvars var, t_list **tmp, t_list **env, t_cmds *ptr)
{
	var.i = 0;
	*tmp = *env;
	while (ptr->option[var.j][var.i] != '=')
		var.i++;
	while (*tmp && ft_strncmpp((*tmp)->content, ptr->option[var.j], var.i))
		*tmp = (*tmp)->next;
}

int	export_and_files(t_cmds *ptr, t_list **env, int fd)
{
	if (ptr->files)
		(open_files(ptr), dup_fds(ptr));
	sigs.exit_s = 0;
	if (!ptr->option[0])
	{
		(print_export(env), dup2(fd, 1));
		close(fd);
		return (1);
	}
	return (0);
}

void	export_while(t_cmds *ptr, t_bvars var, t_list *tmp, t_list **env)
{
	while (ptr->option[var.j])
	{
		if (!valid_identifier(ptr->option[var.j]))
		{
			identifier_error(ptr->option[var.j]);
			var.j++;
			continue ;
		}
		if (append_value(ptr->option[var.j]))
			export_helper2(var, tmp, ptr, env);
		else if (change_value(ptr->option[var.j]))
		{
			export_helper3(var, &tmp, env, ptr);
			if (!tmp)
			{
				ft_lstadd_backk(env, ft_lstneww(ptr->option[var.j]));
				continue ;
			}
			tmp->content = ft_strdup(ptr->option[var.j]);
		}
		else
			export_helper(var, ptr, tmp, env);
		var.j++;
	}
}

void	execute_export(t_list *cmd, t_list **env)
{
	t_cmds	*ptr;
	t_bvars	var;
	t_list	*tmp;
	int		fd;

	ptr = (t_cmds *)cmd->content;
	init_bvars(&var);
	tmp = *env;
	fd = dup(1);
	if (export_and_files(ptr, env, fd))
		return ;
	export_while(ptr, var, tmp, env);
	(dup2(fd, 1), close(fd));
}

int	unset_helper(t_list **curr, t_cmds *ptr, t_list **env, int i)
{
	int	fd;

	fd = dup(1);
	*curr = *env;
	if (*curr && !ft_strncmpp((*curr)->content, ptr->option[i],
			ft_strlenn(ptr->option[i])))
	{
		*env = (*curr)->next;
		(free(curr), dup2(fd, 1));
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	unset_helper1(t_list **curr, t_list **prev, int fd)
{
	if (*curr == NULL)
	{
		(dup2(fd, 1), close(fd));
		return (1);
	}
	(*prev)->next = (*curr)->next;
	free(*curr);
	return (0);
}

void	sub_while(t_list **curr, t_cmds *ptr, int i, t_list **prev)
{
	while (*curr && ft_strncmpp((*curr)->content, ptr->option[i],
			ft_strlenn(ptr->option[i])))
	{
		*prev = *curr;
		*curr = (*curr)->next;
	}
}

int	unset_while(t_cmds *ptr, t_list *curr, t_list *prev, t_list **env)
{
	int	i;
	int	fd;

	i = 0;
	fd = dup(1);
	while (ptr->option[i])
	{
		if (!unset_valid(ptr->option[i]))
		{
			identifier_error(ptr->option[i]);
			i++;
			continue ;
		}
		prev = NULL;
		if (unset_helper(&curr, ptr, env, i))
			return (1);
		sub_while(&curr, ptr, i, &prev);
		if (unset_helper1(&curr, &prev, fd))
			return (1);
		i++;
	}
	return (close(fd), 0);
}

void	execute_unset(t_list *cmd, t_list **env)
{
	t_list	*curr;
	t_list	*prev;
	t_cmds	*ptr;
	int		fd;

	ptr = (t_cmds *)cmd->content;
	fd = dup(1);
	if (ptr->files)
		(open_files(ptr), dup_fds(ptr));
	sigs.exit_s = 0;
	prev = NULL;
	curr = NULL;
	if (!ptr->option)
	{
		(dup2(fd, 1), close(fd));
		return ;
	}
	if (unset_while(ptr, curr, prev, env))
		return ;
	(dup2(fd, 1), close(fd));
}

void	write_exit(void)
{
	if (sigs.process == 0)
		write(2, "exit\n", 6);
}

void	write_and_exit(int exit_status, t_list **env)
{
	write_exit();
	shlvl_edit(env, 1);
	exit(exit_status);
}

void	check_exit_status(int exit_status, t_list **env)
{
	if (exit_status < 0)
	{
		exit_status += 255;
		write_and_exit(exit_status, env);
	}
	else if (exit_status > 255)
	{
		exit_status -= 256;
		write_and_exit(exit_status, env);
	}
	else
		write_and_exit(exit_status, env);
}

void	check_exit_errors(t_cmds *ptr, t_list **env)
{
	if (!ptr->option[0])
	{
		write_exit();
		shlvl_edit(env, 1);
		exit(0);
	}
	if (!is_digit(ptr->option[0]))
	{
		write_exit();
		shlvl_edit(env, 1);
		msg_exit(ptr->option[0], ": numeric argument required\n", 255);
	}
}

void	execute_exit(t_list *cmd, t_list **env)
{
	t_cmds	*ptr;
	int		exit_status;
	int		i;
	int		fd;

	i = 0;
	ptr = (t_cmds *)cmd->content;
	fd = dup(1);
	if (ptr->files)
		(open_files(ptr), dup_fds(ptr));
	check_exit_errors(ptr, env);
	while (ptr->option[i])
		i++;
	if (i > 1)
	{
		write(2, "exit\n", 6);
		(write(2, "exit: too many arguments\n", 25), dup2(fd, 1));
		sigs.exit_s = 1;
		return ;
	}
	exit_status = ft_atoi(ptr->option[0]);
	(check_exit_status(exit_status, env), dup2(fd, 1));
}

void	exportt(char *value, char *var, t_list **env)
{
	t_list		*tmp;
	static int	i;

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
	char		*pwd;
	t_list		*tmp;
	static int	i;

	pwd = getcwd(NULL, 0);
	tmp = *env;
	while (tmp && ft_strncmpp(tmp->content, "PWD=", 4))
		tmp = tmp->next;
	if (!pwd)
		return ;
	if (i > 0)
		free(tmp->content);
	tmp->content = ft_strjoinn("PWD=", pwd);
	if (pwd)
		free(pwd);
	i++;
}

int	check_for_permission(t_cmds *ptr, int fd)
{
	if (access(ptr->option[0], F_OK))
	{
		(write(2, "Minishell: cd: No such file or directory\n", 41), dup2(fd,
				1));
		sigs.exit_s = 1;
		close(fd);
		return (1);
	}
	else if (access(ptr->option[0], X_OK))
	{
		(write(2, "Minishell: cd: Permission Denied!\n", 21), dup2(fd, 1));
		sigs.exit_s = 1;
		close(fd);
		return (1);
	}
	return (0);
}

int	find_home(t_list *tmp, int fd)
{
	while (tmp && ft_strncmpp(tmp->content, "HOME=", 5))
		tmp = tmp->next;
	if (!tmp)
	{
		(write(2, "cd: HOME not set\n", 17), dup2(fd, 1));
		sigs.exit_s = 1;
		close(fd);
		return (1);
	}
	return (0);
}

void	chdir_and_pwd(char *dir, int fd, t_list **env)
{
	chdir(dir);
	dup2(fd, 1);
	setpwd(env);
	close(fd);
}

void	execute_cd(t_list *cmd, t_list **env)
{
	t_cmds	*ptr;
	t_list	*tmp;
	char	*dir;
	int		fd;

	ptr = (t_cmds *)cmd->content;
	tmp = *env;
	fd = dup(1);
	dir = getcwd(NULL, 0);
	if (dir)
		exportt(dir, "OLDPWD=", env);
	if (ptr->files)
		(open_files(ptr), dup_fds(ptr));
	sigs.exit_s = 0;
	if (!ptr->option[0])
	{
		if (find_home(tmp, fd))
			return ;
		chdir_and_pwd(tmp->content + 5, fd, env);
		return ;
	}
	if (check_for_permission(ptr, fd))
		return ;
	chdir_and_pwd(ptr->option[0], fd, env);
}

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
		sigs.exit_s = 0;
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
		(execute_echo(cmd), close(fd), sigs.exit_s = 0);
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
