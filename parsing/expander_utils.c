/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 04:33:57 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 04:43:37 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_isalnum(int i)
{
	if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z'))
		return (1);
	else if (i >= '0' && i <= '9')
		return (1);
	else if (i == '_')
		return (1);
	else
		return (0);
}

int	var_length(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		i++;
		count++;
	}
	return (count);
}

char	*env_value(char *str, t_list *env)
{
	t_list	*tmp;
	char	*value;
	int		i;

	if (ft_strcmp(str, "") == 0)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		if (ft_strncmp(str, tmp->content, var_length(tmp->content)) == 0
			&& (var_length(tmp->content) == ft_strlen(str)))
		{
			value = ft_strdup(tmp->content);
			while (value[i] != '=')
				i++;
			free(value);
			return (tmp->content + i + 1);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

int	ft_ischar(int i)
{
	if (i == 64 || i == 42 || i == 33 || i == 63 || (i >= 48 && i <= 57))
		return (1);
	else
		return (0);
}

char	*cmd_expand(char *cmd, int limiter, char *lineup, char *full_str)
{
	if (cmd[limiter])
	{
		lineup = ft_substr(cmd, limiter, 1000);
		free(cmd);
		cmd = ft_strjoin(full_str, lineup);
		free(lineup);
	}
	else
	{
		free(cmd);
		cmd = ft_strdup(full_str);
	}
	return (cmd);
}
