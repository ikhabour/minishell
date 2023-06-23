/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins8.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:14:12 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:14:23 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		g_sigs.exit_s = 1;
		close(fd);
		return (1);
	}
	else if (access(ptr->option[0], X_OK))
	{
		(write(2, "Minishell: cd: Permission Denied!\n", 21), dup2(fd, 1));
		g_sigs.exit_s = 1;
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
		g_sigs.exit_s = 1;
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
	g_sigs.exit_s = 0;
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
