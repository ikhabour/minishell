/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 05:17:31 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 05:32:09 by bhazzout         ###   ########.fr       */
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
        printf("the command name is: (%s)\n", node->cmd_name);
		if (node->files)
		{
    		t_filetype *file_node = (t_filetype *)node->files->content;
			for (int i = 0; (node->files); i++)
			{
				printf("haaaaaa\n");
				printf("the type : %s\n", file_node->type);
				printf("the red : %s\n", file_node->red);
				printf("the file name : %s\n", file_node->file_name);
				printf("has quotes : %d\n", file_node->has_quotes);
				node->files = node->files->next;
				if (node->files)
				{
					file_node = (t_filetype *)node->files->content;
				}
			}
		}
		for(int i = 0; (node->option) && (node->option)[i] ; i++)
		{
			printf("The option is : %s\n", (node->option)[i]);
		}
        tmp = tmp->next;
    }
}

void	array_printer(int *input)
{
	int i;

	i = 0;
	while (input[i])
	{
		printf("%d\n", input[i]);
		i++;
	}
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
