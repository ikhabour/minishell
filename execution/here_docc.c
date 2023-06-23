/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 15:02:45 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:19:11 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_files(t_list *commands)
{
	t_list		*tmp;
	t_list		*tmp1;
	t_filetype	*files;
	t_cmds		*ptr;

	tmp1 = commands;
	ptr = (t_cmds *)tmp1->content;
	while (tmp1)
	{
		if (!ptr->files)
			return ;
		tmp = ptr->files;
		files = (t_filetype *)tmp->content;
		while (tmp)
		{
			if (files->fd != -1 && files->fd != 0)
				close(files->fd);
			tmp = tmp->next;
			if (tmp)
				files = (t_filetype *)tmp->content;
		}
		tmp1 = tmp1->next;
		if (tmp1)
			ptr = (t_cmds *)tmp1->content;
	}
}

int	is_heredoc(t_list *commands)
{
	t_cmds		*ptr;
	t_filetype	*files;
	t_list		*tmp;

	ptr = (t_cmds *)commands->content;
	if (!ptr->files)
		return (0);
	tmp = ptr->files;
	files = (t_filetype *)tmp->content;
	while (tmp)
	{
		if (!ft_strcmp(files->type, "DELIMITER"))
			return (1);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}

int	limiter_value_ex(int *i, char *cmd)
{
	int	limiter;

	limiter = *i + 1;
	while (cmd[limiter] && (ft_isalnum(cmd[limiter])
			|| ft_ischar(cmd[limiter])))
		limiter++;
	return (limiter);
}

char	*cmd_expand_ex(char *cmd, int limiter, char *lineup, char *full_str)
{
	if (cmd[limiter])
	{
		lineup = ft_substr(cmd, limiter, 1000);
		free(cmd);
		cmd = ft_strjoin(full_str, lineup);
		free(lineup);
	}
	else
	{
		free(cmd);
		cmd = ft_strdup(full_str);
	}
	return (cmd);
}
