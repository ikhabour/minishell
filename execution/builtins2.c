/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:09:58 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:10:11 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		(dup2(fd, 1), close(fd), g_sigs.exit_s = 0);
		return ;
	}
	(printf("env: %s: No such file or directory\n", ptr->option[0]), dup2(fd,
			1));
	close(fd);
	g_sigs.exit_s = 127;
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
