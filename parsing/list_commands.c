/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 14:46:29 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/05 23:32:29 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	split_print(char **input)
{
	int i;

	i = 0;
	while (input[i])
	{
		printf("the array contain this : %s\n", input[i]);
		i++;
	}
}

void	print_list(t_list *list)
{
	t_list *tmp = list;

    while (tmp)
    {
		printf("==================\n");
    	t_cmds *node = (t_cmds *)tmp->content;
    	t_filetype *file_node = (t_filetype *)node->files->content;
        printf("the command name is: %s\n", node->cmd_name);
		for (int i = 0; (node->files); i++)
		{
			printf("the type : %s\n", file_node->type);
			printf("the red : %s\n", file_node->red);
			printf("the file name : %s\n", file_node->file_name);
			node->files = node->files->next;
			if (node->files)
			{
    			file_node = (t_filetype *)node->files->content;
			}
		}
		for(int i = 0; (node->option) && (node->option)[i] ; i++)
		{
			printf("The option is : %s\n", (node->option)[i]);
		}
        tmp = tmp->next;
    }
}

t_filetype	*fill_file(char **cmd_array, int *arr, int i)
{
	t_filetype	*file_node;

	file_node = malloc (sizeof(t_filetype));
	file_node->red = NULL;
	file_node->type = NULL;
	file_node->file_name = NULL;
	if (arr[i] == R_IN_OUT)
	{
		file_node->type = ft_strdup("OUTPUT");
		file_node->red = ft_strdup(cmd_array[i]);
		file_node->file_name = ft_strdup(cmd_array[i + 1]);
	}
	// else if (arr[i] == R_IN_SIG)
	// 	file_node->red = ft_strdup(cmd_array[i]);
	// else if (arr[i] == R_OUT_SIG)
	// 		file_node->red = ft_strdup(cmd_array[i]);
	else if (arr[i] == R_OUT_FILE)
	{
		file_node->type = ft_strdup("OUTPUT");
		file_node->file_name = ft_strdup(cmd_array[i]);
		file_node->red = ft_strdup(cmd_array[i - 1]);
		// file_node->fd = open(file_node->file_name, O_CREAT, O_RDWR);
	}
	// else if (arr[i] == R_APP_SIG)
	// 	file_node->red = ft_strdup(cmd_array[i]);
	else if (arr[i] == R_APP_FILE)
	{
		file_node->type = ft_strdup("APPEND");
		file_node->file_name = ft_strdup(cmd_array[i]);
		file_node->red = ft_strdup(cmd_array[i - 1]);
		// file_node->fd = open(file_node->file_name, O_CREAT, O_RDWR);
	}
	else if (arr[i] == R_IN_FILE)
	{
		file_node->type = ft_strdup("INPUT");
		file_node->file_name = ft_strdup(cmd_array[i]);
		file_node->red = ft_strdup(cmd_array[i - 1]);
		// file_node->fd = open(file_node->file_name, O_CREAT, O_RDONLY);
	}
	// else if (arr[i] == HEREDOC_SIG)
	// 	file_node->red = ft_strdup(cmd_array[i]);
	else if (arr[i] == HEREDOC_LIM)
	{
		printf("haaaa\n");
		file_node->type = ft_strdup("DELIMITER");
		file_node->file_name = ft_strdup(cmd_array[i]);
		file_node->red = ft_strdup(cmd_array[i - 1]);
		// node->files.fd = open(node->files.file_name, O_CREAT, O_RDWR);
	}
	return (file_node);
}

void	node_printer(t_list *file_node)
{
	t_list *tmp = file_node;
	
	while (tmp)
	{
		t_filetype *file = (t_filetype *)tmp->content;
		// for (int i = 0; file; i++)
		// {
			printf("this is the content (%s)\n", file->file_name);
			// file = file->next;
		// }
		// printf("haaaa\n");
		tmp = tmp->next;
	}
}

t_cmds	*fill_node(char **cmd_array, int *arr, int i)
{
	int	count;
	int	arg_counter = 0;
	int	index = i;

	t_cmds		*node;
	t_list		*file = NULL;
	t_filetype	*file_node = NULL;

	count = 0;
	node = malloc (sizeof (t_cmds));
	node->cmd_name = NULL;
	node->option = NULL;

	while (index >= 0 && arr[index] != PIPE)
	{
		if (arr[index] == CMD_ARG)
			arg_counter++;
		index--;
	}
	if (arg_counter > 0)
	{
		node->option = malloc ((arg_counter + 1) * sizeof(char *));
		node->option[arg_counter] = NULL;
	}
	while (i >= 0 && arr[i] != PIPE)
	{
		if (arr[i] == CMD_NAME)
		{
			node->cmd_name = ft_strdup(cmd_array[i]);
		}	
		else if (arr[i] == CMD_ARG)
		{
			node->option[arg_counter - 1] = ft_strdup(cmd_array[i]);
			arg_counter--;
		}
		else if (arr[i] == R_IN_FILE || arr[i] == R_OUT_FILE || arr[i] == R_APP_FILE
				|| arr[i] == HEREDOC_LIM)
		{
			file_node = fill_file(cmd_array, arr, i);
			ft_lstadd_front(&file, my_lstnew(file_node));
		}
		i--;
	}
	node->files = file;
	return (node);
}

t_list	*list_cmds(char **cmd_array, int *arr)
{
	int		i;
	t_list *list = NULL;
	// (void) cmd_array;
	t_cmds	*node = NULL;

	i = 0;
	// list = malloc (sizeof(t_list));
	while (arr[i])
	{
		if (arr[i] == PIPE)
		{
			node = fill_node(cmd_array, arr, (i - 1));
			my_lstadd_back(&list, my_lstnew(node));
			node = NULL;
		}
		i++;
		if (arr[i] == '\0')
		{
			node = fill_node(cmd_array, arr, (i - 1));
			my_lstadd_back(&list, my_lstnew(node));
			node = NULL;
		}
	}
	// print_list(list);
	return (list);
}