/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:21:53 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:42:58 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	dup_input_file(t_cmds *ptr)
{
	t_filetype	*files;
	t_list		*tmp;

	if (!ptr->files)
		return ;
	tmp = ptr->files;
	files = (t_filetype *)tmp->content;
	while (tmp)
	{
		if (files->fd == -1)
		{
			write(2, "Minishell: ", 11);
			write(2, files->file_name, ft_strlenn(files->file_name));
			write(2, ": No such file or directory\n", 28);
			return ;
		}
		if (!ft_strcmp(files->type, "INPUT") || !ft_strcmp(files->type,
				"DELIMITER"))
			dup2(files->fd, 0);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
}

int	output_file(t_cmds *ptr)
{
	t_list		*tmp;
	t_filetype	*files;

	if (!ptr->files)
		return (0);
	tmp = ptr->files;
	files = (t_filetype *)tmp->content;
	while (tmp)
	{
		if (!ft_strcmp(files->type, "OUTPUT") || !ft_strcmp(files->type,
				"APPEND"))
			return (1);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
	return (0);
}

void	dup_output_file(t_cmds *ptr)
{
	t_filetype	*files;
	t_list		*tmp;

	if (!ptr->files)
		return ;
	tmp = ptr->files;
	files = (t_filetype *)tmp->content;
	while (tmp)
	{
		if (!ft_strcmp(files->type, "OUTPUT") || !ft_strcmp(files->type,
				"APPEND"))
			dup2(files->fd, 1);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
}

void	subpipes_helper(t_cmds *ptr, t_list *commands, t_list **env, int *fd)
{
	if (!open_files(ptr))
		exit(1);
	if (input_file(ptr))
		dup_input_file(ptr);
	if (output_file(ptr))
		dup_output_file(ptr);
	else
		dup2(fd[1], 1);
	close(fd[0]);
	if (!ptr->cmd_name)
		exit(0);
	if (execute_builtins(commands, env))
		exit(0);
}

void	subpipes_helper2(t_cmds *ptr, t_list *commands, t_list **env, int *fd)
{
	if (!open_files(ptr))
		exit(1);
	if (output_file(ptr))
		dup_output_file(ptr);
	if (input_file(ptr))
		dup_input_file(ptr);
	else
		dup2(fd[0], 0);
	close(fd[1]);
	if (!ptr->cmd_name)
		exit(0);
	if (execute_builtins(commands, env))
		exit(0);
}
