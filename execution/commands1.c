/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:15:14 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:15:38 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_len(t_list **env)
{
	int		i;
	t_list	*tmp;

	i = 0;
	tmp = *env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

char	**env_to_array(t_list **env)
{
	t_list	*tmp;
	int		i;
	char	**array;

	tmp = *env;
	i = count_len(env);
	array = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (tmp)
	{
		array[i++] = ft_strdupp(tmp->content);
		tmp = tmp->next;
	}
	array[i] = NULL;
	return (array);
}

char	**get_path(char **envp)
{
	int		i;
	char	**paths;
	char	*save;

	i = 0;
	while (envp[i] && ft_strncmpp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		save = paths[i];
		paths[i] = ft_strjoinn(paths[i], "/");
		free(save);
		i++;
	}
	return (paths);
}

int	get_len(char **arg)
{
	int	i;

	i = 0;
	while (arg[i])
		i++;
	return (i);
}

void	msg_exit(char *msg, char *msg1, int status)
{
	write(2, msg, ft_strlenn(msg));
	write(2, msg1, ft_strlenn(msg1));
	exit(status);
}
