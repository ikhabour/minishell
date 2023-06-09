/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_processor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 23:53:47 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 01:52:16 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_name(char *env)
{
	int		i;
	char	*name;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	name = malloc(i + 1);
	i = 0;
	while (env[i] && env[i] != '=')
	{
		name[i] = env[i];
		i++;
	}
	name[i] = '\0';
	return (name);
}

char	*get_env_value(char *env)
{
	char	*value;

	value = ft_strchr(env, '=');
	if (!value)
		return (NULL);
	return (value + 1);
}

t_env	*get_env(char **env)
{
	int		i;
	char	*name;
	char	*value;
	t_env	*new;
	t_env	*head;

	i = 0;
	head = NULL;
	while (env[i])
	{
		new = malloc(sizeof(t_env));
		name = get_env_name(env[i]);
		value = get_env_value(env[i]);
		new = ft_lstnew(name, value, new);
		ft_lstadd_back(&head, new);
		i++;
	}
	return (head);
}
