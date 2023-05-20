/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:31:49 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/20 17:58:55 by ikhabour         ###   ########.fr       */
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

void	execute_echo(t_list *cmd)
{
	t_bvars	var;
	t_cmds	*tmp;

	var.n = 0;
	var.j = 0;
	tmp = (t_cmds *)cmd->content;
	var.i = 0;
	if (!tmp->option)
	{
		printf(" \n");
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

	tmp = *env;
	ptr = (t_cmds *)cmd->content;
	if (!ptr->option)
	{
		while (tmp)
		{
			if (change_value(tmp->content))
				printf("%s\n", tmp->content);
			tmp = tmp->next;
		}
		return ;
	}
	printf("env: %s: No such file or directory\n", ptr->option[0]);
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

void	execute_export(t_list *cmd, t_list **env)
{
	t_cmds	*ptr;
	t_bvars var;
	t_list	*tmp;
	char *temp;

	ptr = (t_cmds *)cmd->content;
	var.i = 0;
	var.j = 0;
	var.n = 0;
	tmp = *env;
	if (!ptr->option)
	{
		print_export(env);
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
				continue ;
			}
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
			tmp->content = ptr->option[var.j];
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
}

void	execute_unset(t_list *cmd, t_list **env)
{
	t_list	*curr;
	t_list	*prev;
	t_cmds	*ptr;
	int i;

	ptr = (t_cmds *)cmd->content;
	i = 0;
	if (!ptr->option)
	{
		printf("unset: not enough argumenets\n");
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
			free(curr);
			return ;
		}
		while (curr && ft_strncmpp(curr->content, ptr->option[i],
				ft_strlenn(ptr->option[i])))
		{
			prev = curr;
			curr = curr->next;
		}
		if (curr == NULL)
			return ;
		prev->next = curr->next;
		free(curr);
		i++;
	}
}

void	execute_exit(t_list *cmd)
{
	t_cmds	*ptr;
	int		exit_status;

	ptr = (t_cmds *)cmd->content;
	if (!ptr->option)
		exit(0);
	if (!is_digit(ptr->option[0]))
	{
		printf("exit: %s: numeric argument required\n", ptr->option[0]);
		exit(255);
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
}

void	execute_cd(t_list *cmd, t_list **env)
{
	t_cmds *ptr;
	t_list *tmp;

	ptr = (t_cmds *)cmd->content;
	tmp = *env;
	if (!ptr->option)
	{
		while (tmp->content && ft_strncmpp(tmp->content, "HOME=", 5))
			tmp = tmp->next;
		chdir(tmp->content + 5);
	}
	else
	{
		if (chdir(ptr->option[0]) == -1)
			printf("cd: no such file or directory: %s\n", ptr->option[0]);
	}

}

int	execute_builtins(t_list *cmd, t_list **env)
{
	t_cmds	*tmp;

	tmp = (t_cmds *)cmd->content;
	if (!ft_strcmpp(tmp->cmd_name, "pwd"))
	{
		tmp->cmd_name = getcwd(NULL, 0);
		printf("%s\n", tmp->cmd_name);
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