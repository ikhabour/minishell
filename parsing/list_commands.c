/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 14:46:29 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/01 19:05:11 by ikhabour         ###   ########.fr       */
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
	// t_filetype *file_node = NULL;

    while (tmp)
    {
		printf("==================\n");
    	t_cmds *node = (t_cmds *)tmp->content;
        printf("the command name is: %s\n", node->cmd_name);
		// if (node->files.fd < 0)
		// 		printf("Can not create fd.\n");
		// else
			// printf("the fd is: %d\n", node->files.fd);
		t_filetype *file_node = (t_filetype *)node->files;
		for (int i = 0; (node->files); i++)
		{
			printf("the red : %s\n", file_node->red);
			printf("the type : %s\n", file_node->type);
			printf("the file name : %s\n", file_node->file_name);
			file_node = file_node->next;
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
		file_node->red = ft_strdup(cmd_array[i]);
	else if (arr[i] == R_IN_SIG)
		file_node->red = ft_strdup(cmd_array[i]);
	else if (arr[i] == R_OUT_SIG)
		file_node->red = ft_strdup(cmd_array[i]);
	else if (arr[i] == R_OUT_FILE)
	{
		file_node->type = ft_strdup("OUTPUT");
		file_node->file_name = ft_strdup(cmd_array[i]);
		// file_node->fd = open(file_node->file_name, O_CREAT, O_RDWR);
	}
	else if (arr[i] == R_APP_SIG)
		file_node->red = ft_strdup(cmd_array[i]);
	else if (arr[i] == R_APP_FILE)
	{
		file_node->type = ft_strdup("APPEND");
		file_node->file_name = ft_strdup(cmd_array[i]);
		// file_node->fd = open(file_node->file_name, O_CREAT, O_RDWR);
	}
	else if (arr[i] == R_IN_FILE)
	{
		file_node->type = ft_strdup("INPUT");
		file_node->file_name = ft_strdup(cmd_array[i]);
		// file_node->fd = open(file_node->file_name, O_CREAT, O_RDONLY);
	}
	else if (arr[i] == HEREDOC_SIG)
		file_node->red = ft_strdup(cmd_array[i]);
	else if (arr[i] == HEREDOC_LIM)
	{
		file_node->type = ft_strdup("DELIMITER");
		file_node->file_name = ft_strdup(cmd_array[i]);
		// node->files.fd = open(node->files.file_name, O_CREAT, O_RDWR);
	}
	return (file_node);
}

// void	node_printer(t_filetype *file_node)
// {
// 	t_list *tmp = file_node;

//     while (tmp)
//     {
// 		printf("==================\n");
//     	t_cmds *node = (t_cmds *)tmp->content;
// 		// // split_print(node->option);
// 		// printf("The type is : %s\n", node->files->type);
// 		// printf("The red is : %s\n", node->files->red);
// 		// printf("The file_name is : %s\n", node->files->file_name);
//         printf("the command name is: %s\n", node->cmd_name);
// 		// if (node->files.fd < 0)
// 		// 		printf("Can not create fd.\n");
// 		// else
// 			// printf("the fd is: %d\n", node->files.fd);
		
// 		for(int i = 0;(node->option) && (node->option)[i] ; i++)
// 		{
// 			printf("The option is : %s\n", (node->option)[i]);
// 		}
//         tmp = tmp->next;
//     }
// }

t_cmds	*fill_node(char **cmd_array, int *arr, int i)
{
	int	count;
	int	j;
	int	index = 0;
	int	start;

	t_cmds		*node;
	t_list		*file = NULL;
	t_filetype	*file_node;

	count = 0;
	node = malloc (sizeof (t_cmds));
	// file_node = malloc (sizeof(t_filetype));
	file_node = NULL;
	file_node->red = NULL;
	file_node->type = NULL;
	file_node->file_name = NULL;
	node->cmd_name = NULL;
	node->option = NULL;
	// node->files->fd = -1;

	while (i >= 0 && arr[i] != PIPE)
	{
		if (arr[i] == CMD_NAME)
		{
			node->cmd_name = ft_strdup(cmd_array[i]);
		}	
		else if (arr[i] == CMD_ARG)
		{
			count = 0;
			j = i;
			while (i > 0 && arr[i] == CMD_ARG)
			{
				i--;
				count++;
				if (arr[i] != CMD_ARG)
				{
					i++;
					start = i;
					break ;
				}
			}
			if (count > 0)
			{
				node->option = malloc ((count + 1) * sizeof(char *));
				node->option[count] = NULL;
				while (index < count)
				{
					node->option[index] = ft_strdup(cmd_array[start]);
					index++;
					start++;
				}
			}
		}
		else
		{
			// printf("haaaaaa\n");
			file_node = fill_file(cmd_array, arr, i);
			// printf("====(%s)===\n", file_node->file_name);
			my_lstadd_back(&file, my_lstnew(file_node));
		}
		i--;
	}
	// node_printer(file_node);
	node->files = (t_filetype *)file;
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
			printf("(%d)\n", arr[i - 1]);
			node = fill_node(cmd_array, arr, (i - 1));
			my_lstadd_back(&list, my_lstnew(node));
			node = NULL;
		}
	}
	print_list(list);
	return (list);
}