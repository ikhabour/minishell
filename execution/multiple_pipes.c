/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 15:40:24 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:21:46 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_pipes(t_list *commands)
{
	int	pipe;

	pipe = -1;
	while (commands)
	{
		pipe++;
		commands = commands->next;
	}
	return (pipe);
}

char	**make_argv(t_list *commands)
{
	char	**argv;
	t_cmds	*ptr;
	int		i;
	int		j;

	ptr = (t_cmds *)commands->content;
	i = 0;
	j = 0;
	if (!ptr->cmd_name)
		return (NULL);
	if (!ptr->option)
	{
		argv = malloc(sizeof(char *) * 2);
		argv[0] = ptr->cmd_name;
		argv[1] = NULL;
		return (argv);
	}
	while (ptr->option[i])
		i++;
	argv = malloc(sizeof(char *) * (i + 2));
	argv[0] = ft_strdup(ptr->cmd_name);
	i = 1;
	while (ptr->option[j])
		argv[i++] = ft_strdup(ptr->option[j++]);
	return (argv[i] = NULL, argv);
}

int	input_file(t_cmds *ptr)
{
	t_list		*tmp;
	t_filetype	*files;

	if (!ptr->files)
		return (0);
	tmp = ptr->files;
	files = (t_filetype *)tmp->content;
	while (tmp)
	{
		if (!ft_strcmp(files->type, "INPUT") || !ft_strcmp(files->type,
				"DELIMITER"))
			return (1);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
	return (0);
}
