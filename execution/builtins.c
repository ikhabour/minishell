/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:31:49 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/17 23:32:31 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_echo_option(char *str, t_bvars *var)
{
	int	i;

	i = 0;
	// printf("str : %s\n", str);
	if (str[i] == '-')
	{
		i++;
		if (!str[i])
			return (1);
		while (str[i])
		{
			// printf("was here \n");
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
		ft_lstadd_back(&env, ft_lstnew(envp[i++]));
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
	if (ptr->option[0])
	{
		printf("env: %s: No such file or directory\n", ptr->option[0]);
		exit(127);
	}
	while (tmp)
	{
		if (change_value(tmp->content))
			printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
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

void	execute_export(t_list *cmd, t_list **env)
{
	t_cmds	*ptr;
	int		i;
	t_list	*tmp;
	char *temp;

	ptr = (t_cmds *)cmd->content;
	i = 0;
	tmp = *env;
	if (!ptr->option[0])
	{
		while (tmp)
		{
			printf("declare -x %s\n", tmp->content);
			tmp = tmp->next;
		}
		return ;
	}
	if (append_value(ptr->option[0]))
	{
		i = 0;
		tmp = *env;
		while (ptr->option[0][i] != '=')
			i++;
		while (tmp && ft_strncmp(tmp->content, ptr->option[0], i - 1))
			tmp = tmp->next;
		if (!tmp)
		{
			ft_lstadd_back(env, ft_lstnew(remove_plus(ptr->option[0])));
			return ;
		}
		tmp->content = ft_strjoinn(tmp->content, ptr->option[0] + i + 1);
	}
	else if (change_value(ptr->option[0]))
	{
		i = 0;
		tmp = *env;
		while (ptr->option[0][i] != '=')
			i++;
		while (tmp && ft_strncmp(tmp->content, ptr->option[0], i))
			tmp = tmp->next;
		if (!tmp)
		{
			ft_lstadd_back(env, ft_lstnew(ptr->option[0]));
			return ;
		}
		tmp->content = ptr->option[0];
	}
	else
		ft_lstadd_back(env, ft_lstnew(ptr->option[0]));
}

void	execute_unset(t_list *cmd, t_list **env)
{
	t_list	*curr;
	t_list	*prev;
	t_cmds	*ptr;

	curr = *env;
	prev = NULL;
	ptr = (t_cmds *)cmd->content;
	if (curr && !ft_strncmp(curr->content, ptr->option[0],
			ft_strlenn(ptr->option[0])))
	{
		*env = curr->next;
		free(curr);
		return ;
	}
	while (curr && ft_strncmp(curr->content, ptr->option[0],
			ft_strlenn(ptr->option[0])))
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr == NULL)
		return ;
	prev->next = curr->next;
	free(curr);
}

void	execute_exit(t_list *cmd)
{
	t_cmds	*ptr;
	int		exit_status;

	ptr = (t_cmds *)cmd->content;
	if (!ptr->option[0])
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

void	execute_builtins(t_list *cmd, t_list **env)
{
	t_cmds	*tmp;

	tmp = (t_cmds *)cmd->content;
	if (!ft_strcmp(tmp->cmd_name, "pwd"))
	{
		tmp->cmd_name = getcwd(NULL, 0);
		printf("%s\n", tmp->cmd_name);
	}
	else if (!ft_strcmp(tmp->cmd_name, "echo"))
		execute_echo(cmd);
	else if (!ft_strcmp(tmp->cmd_name, "cd"))
		chdir(tmp->option[0]);
	else if (!ft_strcmp(tmp->cmd_name, "env"))
		execute_env(env, cmd);
	else if (!ft_strcmp(tmp->cmd_name, "export"))
		execute_export(cmd, env);
	else if (!ft_strcmp(tmp->cmd_name, "unset"))
		execute_unset(cmd, env);
	else if (!ft_strcmp(tmp->cmd_name, "exit"))
		execute_exit(cmd);
	// execute_env(env);
	// system("leaks a.out");
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_list *head;
	t_cmds var;
	t_list *env;
	char *s;
	char **split;
	int i;

	env = make_env(envp);
	while (1)
	{
		s = readline("minishell>");
		if (!s || !s[0])
			break ;
		split = ft_split(s, ' ');
		var.cmd_name = ft_strdupp(split[0]);
		var.option = dup_2d(split + 1);
		var.files.file_name = NULL;
		var.files.type = NULL;
		head = ft_lstnew(&var);
		execute_builtins(head, &env);
		i = 0;
		while (split[i])
		{
			free(split[i]);
			i++;
		}
		free(split);
		free(s);
	}
}