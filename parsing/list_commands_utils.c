/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_commands_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 05:34:58 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 05:35:00 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_in_out(char **cmd_array, int i, t_filetype *file_node)
{
	file_node->type = ft_strdup("OUTPUT");
	file_node->red = ft_strdup(cmd_array[i]);
	file_node->file_name = ft_strdup(cmd_array[i + 1]);
}

void	fill_out_file(char **cmd_array, int i, t_filetype *file_node)
{
	file_node->type = ft_strdup("OUTPUT");
	file_node->file_name = ft_strdup(cmd_array[i]);
	file_node->red = ft_strdup(cmd_array[i - 1]);
}

void	fill_app_file(char **cmd_array, int i, t_filetype *file_node)
{
	file_node->type = ft_strdup("APPEND");
	file_node->file_name = ft_strdup(cmd_array[i]);
	file_node->red = ft_strdup(cmd_array[i - 1]);
}

void	fill_in_file(char **cmd_array, int i, t_filetype *file_node)
{
	file_node->type = ft_strdup("INPUT");
	file_node->file_name = ft_strdup(cmd_array[i]);
	file_node->red = ft_strdup(cmd_array[i - 1]);
}

t_filetype	*fill_file(char **cmd_array, int *arr, int i, int *delimiter)
{
	t_filetype	*file_node;

	file_node = malloc (sizeof(t_filetype));
	file_node->red = NULL;
	file_node->type = NULL;
	file_node->file_name = NULL;
	file_node->has_quotes = 0;
	if (arr[i] == R_IN_OUT)
		fill_in_out(cmd_array, i, file_node);
	else if (arr[i] == R_OUT_FILE)
		fill_out_file(cmd_array, i, file_node);
	else if (arr[i] == R_APP_FILE)
		fill_app_file(cmd_array, i, file_node);
	else if (arr[i] == R_IN_FILE)
		fill_in_file(cmd_array, i, file_node);
	else if (arr[i] == HEREDOC_LIM)
	{
		file_node->has_quotes = *delimiter;
		file_node->type = ft_strdup("DELIMITER");
		file_node->file_name = ft_strdup(cmd_array[i]);
		file_node->red = ft_strdup(cmd_array[i - 1]);
	}
	return (file_node);
}
