/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 04:34:54 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 04:46:28 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	limiter_value(int *i, char *cmd)
{
	int	limiter;

	limiter = *i + 1;
	while (cmd[limiter] && (ft_isalnum(cmd[limiter])
			|| ft_ischar(cmd[limiter])))
	{
		limiter++;
	}
	return (limiter);
}

int	start_index(char *cmd, int *i)
{
	if ((*i > 0) && (cmd[*i] == '$' || cmd[*i] == '"' || cmd[*i] == '\''))
		*i -= 1;
	return (*i);
}

static char	*ft_expand(char *cmd, t_list *env, int *i)
{
	int			limiter;
	static char	*full_str;
	char		*lineup;
	char		*str;
	char		*value;

	limiter = limiter_value(i, cmd);
	str = ft_substr(cmd, (*i + 1), limiter - (*i + 1));
	value = env_value(str, env);
	if (value)
	{
		lineup = ft_substr(cmd, 0, (*i));
		full_str = ft_strjoin(lineup, value);
	}
	else
	{
		lineup = ft_substr(cmd, 0, (*i));
		full_str = ft_strdup(lineup);
	}
	free(lineup);
	cmd = cmd_expand(cmd, limiter, lineup, full_str);
	*i = ft_strlen(full_str);
	(free(full_str), free(str));
	*i = start_index(cmd, i);
	return (cmd);
}

static char	*ft_expand_exit(char *cmd, int *i)
{
	char	*str;
	char	*lineup;
	char	*value;
	int		limiter;
	char	*full_str;

	limiter = *(i) + 2;
	str = ft_substr(cmd, limiter, 1000);
	value = ft_itoa(exit_s);
	lineup = ft_substr(cmd, 0, *i);
	full_str = ft_strjoin(lineup, value);
	free(lineup);
	free(cmd);
	cmd = ft_strjoin(full_str, str);
	free(full_str);
	free(value);
	free(str);
	if ((*i > 0) && cmd[*i] == '$')
			*i -= 1;
	return (cmd);
}

char	*expand_processor(char *cmd, t_list *env)
{
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
			flag = is_outside(flag, cmd[i]);
		if (cmd[i] && cmd[i + 1] && cmd[i] == '$'
			&& cmd[i + 1] == '$' && flag != 1)
			i++;
		if (cmd[i] && cmd[i + 1] && cmd[i] == '$'
			&& cmd[i + 1] == '?' && flag != 1)
			cmd = ft_expand_exit(cmd, (&i));
		if (cmd[i] && cmd[i + 1] && cmd[i] == '$'
			&& cmd[i + 1] != '?' && flag != 1)
			cmd = ft_expand(cmd, env, (&i));
		if (cmd[i] && cmd[i + 1] && cmd[i] == '$'
			&& cmd[i + 1] == '?' && flag != 1)
			cmd = ft_expand_exit(cmd, (&i));
		if (cmd[i])
			i++;
	}
	return (cmd);
}
