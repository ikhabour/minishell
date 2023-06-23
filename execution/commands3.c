/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:16:29 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:16:42 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_file_type(t_filetype *files)
{
	if (!access(files->file_name, F_OK) && (access(files->file_name, W_OK)
			|| access(files->file_name, R_OK)))
	{
		write(2, "Minishell : ", 12);
		write(2, files->file_name, ft_strlenn(files->file_name));
		write(2, ": Permission denied\n", 20);
		g_sigs.exit_s = 1;
		return (0);
	}
	if (!ft_strcmp(files->type, "INPUT"))
		files->fd = open(files->file_name, O_RDONLY, 0644);
	else if (!ft_strcmp(files->type, "OUTPUT"))
		files->fd = open(files->file_name, O_CREAT | O_TRUNC | O_RDWR, 0644);
	else if (!ft_strcmp(files->type, "APPEND"))
		files->fd = open(files->file_name, O_CREAT | O_APPEND | O_RDWR, 0644);
	else if (!ft_strcmp(files->type, "DELIMITER"))
		return (1);
	else
		files->fd = -1;
	return (1);
}
