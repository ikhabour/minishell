/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:14:21 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 01:27:36 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**get_new_cmd(char **cmd, int count)
{
	int		i;
	int		j;
	char	**new_cmd;

	j = 0;
	i = 0;
	new_cmd = (char **)ft_calloc(sizeof(char *), count + 1);
	new_cmd[count] = NULL;
	while (cmd[j])
	{
		if (ft_strcmp(cmd[j], "") != 0)
		{
			new_cmd[i] = ft_strdup(cmd[j]);
			i++;
		}
		j++;
	}
	return (new_cmd);
}

char	**expander(char **cmd, t_list *env, int *arr)
{
	int		i;
	int		j;
	char	**new_cmd;
	int		count;

	count = 0;
	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (ft_strchr(cmd[i], '$') && arr[i] != HEREDOC_LIM)
			cmd[i] = expand_processor(cmd[i], env);
		i++;
	}
	while (cmd[j])
	{
		if (ft_strcmp(cmd[j], "") != 0)
			count++;
		j++;
	}
	new_cmd = get_new_cmd(cmd, count);
	free_2d(cmd);
	return (new_cmd);
}
