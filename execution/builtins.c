/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:31:49 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/17 00:54:53 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_echo_option(char *str, t_bvars *var)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i])
		{
			if (str[i] == 'n')
				i++;
			else
				return (1);
		}
	}
	var->n++;
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
	while (!check_echo_option(tmp->option[var.j], &var))
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

void	execute_env(t_list *env)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->next;
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

void	execute_export(t_list *cmd, t_list *env)
{
	t_cmds	*ptr;
	int		i;
	t_list	*tmp;

	ptr = (t_cmds *)cmd->content;
	i = 0;
	tmp = env;
	if (append_value(ptr->option[0]))
	{
		i = 0;
		tmp = env;
		while (ptr->option[0][i] != '=')
			i++;
		while (tmp && ft_strncmp(tmp->content, ptr->option[0], i - 2))
			tmp = tmp->next;
		if (!tmp)
		{
			ft_lstadd_back(&env, ft_lstnew(remove_plus(ptr->option[0])));
			return ;

		}
		tmp->content = ft_strjoinn(tmp->content, ptr->option[0] + i + 1);
	}
	else if (change_value(ptr->option[0]))
	{
		i = 0;
		tmp = env;
		while (ptr->option[0][i] != '=')
			i++;
		while (tmp && ft_strncmp(tmp->content, ptr->option[0], i))
			tmp = tmp->next;
		if (!tmp)
		{
			ft_lstadd_back(&env, ft_lstnew(ptr->option[0]));
			return ;
		}
		tmp->content = ptr->option[0];
	}
}

void	execute_unset(t_list *cmd, t_list *env)
{
	t_list *curr;
	t_list *prev;
	t_cmds *ptr;

	curr = env;
	prev = NULL
	ptr = (t_cmds *)cmd->content;
	while (curr && ft_strcmp(curr->content, ptr->option[0]))
	{
		prev = curr;
		curr = curr->next;
	}
	
	
}

void	execute_builtins(t_list *cmd, t_list *env)
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
		execute_env(env);
	else if (!ft_strcmp(tmp->cmd_name, "export"))
		execute_export(cmd, env);
	else if (!ft_strcmp(tmp->cmd_name, "unset"))
		execute_unset(cmd, env);
	// execute_env(env);
}

int	main(int argc, char **argv, char **envp)
{
	// (void)argc;
	// (void)argv;
	t_list *head;
	t_cmds var;
	t_list *env;

	env = make_env(envp);
	char *strings[] = {argv[2], NULL};
	var.cmd_name = argv[1];
	var.option = strings;
	var.files.file_name = NULL;
	var.files.type = NULL;
	head = ft_lstnew(&var);

	execute_builtins(head, env);
	// system("leaks a.out");
}