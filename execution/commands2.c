/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:15:58 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:16:14 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_files(t_cmds *ptr)
{
	t_filetype	*files;
	t_list		*tmp;

	if (!ptr->files)
		return (1);
	tmp = ptr->files;
	files = (t_filetype *)ptr->files->content;
	while (tmp)
	{
		if (!open_file_type(files))
			return (0);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
	return (1);
}

int	dup_helper(t_filetype *files, int *out, int *in)
{
	if (files->fd == -1)
	{
		msg_exit(files->file_name, ": No such file or directory\n", 1);
		return (0);
	}
	if (!ft_strcmp(files->type, "OUTPUT") || !ft_strcmp(files->type, "APPEND"))
		*out = files->fd;
	else if (!ft_strcmp(files->type, "INPUT") || !ft_strcmp(files->type,
			"DELIMITER"))
		*in = files->fd;
	return (1);
}

void	dup_fds(t_cmds *ptr)
{
	t_filetype	*files;
	t_list		*tmp;
	int			in;
	int			out;

	if (!ptr->files)
		return ;
	files = (t_filetype *)ptr->files->content;
	tmp = ptr->files;
	in = -1;
	out = -1;
	while (tmp)
	{
		if (!dup_helper(files, &out, &in))
			return ;
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
	if (in != -1)
		dup2(in, 0);
	if (out != -1)
		dup2(out, 1);
}

int	has_redirection(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (args[i][0] == '<' || args[i][0] == '>')
			return (1);
		i++;
	}
	return (0);
}

void	wait_process(t_exec *vars)
{
	waitpid(vars->pid, &vars->i, 0);
	g_sigs.exit_s = WEXITSTATUS(vars->i);
	g_sigs.process = 0;
}
