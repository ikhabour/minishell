/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:51:13 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:52:02 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (!str[i])
		return (1);
	return (0);
}

void	free_files(t_list *files)
{
	t_filetype	*file;
	t_list		*tmp;

	file = (t_filetype *)files->content;
	while (files)
	{
		free(file->file_name);
		free(file->type);
		free(file->red);
		tmp = files;
		files = files->next;
		free(tmp);
		free(file);
		if (files)
			file = (t_filetype *)files->content;
	}
}

void	my_free(t_list *commands)
{
	t_cmds	*ptr;
	t_list	*tmp;

	ptr = (t_cmds *)commands->content;
	while (commands)
	{
		free(ptr->cmd_name);
		if (ptr->option)
			free_2d(ptr->option);
		if (ptr->files)
			free_files(ptr->files);
		tmp = commands;
		commands = commands->next;
		free(tmp);
		free(ptr);
		if (commands)
			ptr = (t_cmds *)commands->content;
	}
}

void	free_2d(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	free_all(char *input, char **array)
{
	free(input);
	free_2d(array);
}
