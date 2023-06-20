/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 14:46:29 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 05:42:24 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_arg(int index, int *arr)
{
	int	arg_counter;
	int	i;

	i = index;
	arg_counter = 0;
	while (i >= 0 && arr[i] != PIPE)
	{
		if (arr[i] == CMD_ARG)
			arg_counter++;
		i--;
	}
	return (arg_counter);
}

t_cmds	*node_init(t_vars f_list, int arg_counter)
{
	f_list.node = malloc (sizeof(t_cmds));
	f_list.node->cmd_name = NULL;
	f_list.node->option = NULL;
	f_list.node->option = malloc ((arg_counter + 1) * sizeof(char *));
	f_list.node->option[arg_counter] = NULL;
	return (f_list.node);
}

t_cmds	*fill_node(char **cmd, int *arr, int i, int *delimiter)
{
	t_vars	f_l;

	f_l.file_node = NULL;
	f_l.file = NULL;
	f_l.arg_counter = count_arg(i, arr);
	f_l.node = node_init(f_l, f_l.arg_counter);
	while (i >= 0 && arr[i] != PIPE)
	{
		if (arr[i] == CMD_NAME)
			f_l.node->cmd_name = ft_strdup(cmd[i]);
		else if (arr[i] == CMD_ARG)
		{
			f_l.node->option[f_l.arg_counter - 1] = ft_strdup(cmd[i]);
			f_l.arg_counter--;
		}
		else if (arr[i] == R_IN_FILE || arr[i] == 5 || arr[i] == 8
			|| arr[i] == HEREDOC_LIM)
		{
			f_l.file_node = fill_file(cmd, arr, i, delimiter);
			ft_lstadd_front(&f_l.file, my_lstnew(f_l.file_node));
		}
		i--;
	}
	f_l.node->files = f_l.file;
	return (f_l.node);
}

t_list	*list_cmds(char **cmd_array, int *arr, int *delimiter)
{
	int		i;
	t_list	*list;
	t_cmds	*node;

	list = NULL;
	node = NULL;
	i = 0;
	while (arr[i])
	{
		if (arr[i] == PIPE)
		{
			node = fill_node(cmd_array, arr, (i - 1), delimiter);
			my_lstadd_back(&list, my_lstnew(node));
		}
		i++;
		if (arr[i] == '\0')
		{
			node = fill_node(cmd_array, arr, (i - 1), delimiter);
			my_lstadd_back(&list, my_lstnew(node));
		}
	}
	return (list);
}
