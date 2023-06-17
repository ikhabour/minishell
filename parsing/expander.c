/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:14:21 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/17 00:48:30 by bhazzout         ###   ########.fr       */
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

int	expand_length(char *cmd)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	if (!cmd)
		return (0);
	while (ft_isalnum(cmd[i]))
	{
		i++;
		count++;
	}
	return (count);
}

int	path_index(char *env)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (env[i])
	{
		if (env[i] == '=')
		{
			count++;
			break ;
		}
		i++;
		count++;
	}
	return (count);
}

int	ft_envcmp(char *s1, char *s2, int length)
{
	int	i;

	i = 0;
	while (i < length)
	{
		if (s1[i] != s2[i])
			return (1);
		i++;
	}
	if (s1[i] == '\0' && s2[i] == '\0')
		return (0);
	return (1);
}

char	*get_value(char *cmd, char **env)
{
	// int		i;
	// int		length;
	// int		len;
	(void) env;
	(void) cmd;

	// printf("the str is : (%s)\n", cmd);
	if (ft_strcmp(cmd, ""))
	{
		printf("haaaaaa\n");
		return (NULL);
	}
	// length = expand_length(cmd);
	// i = 0;
	// while (env[i])
	// {
	// 	if (ft_strncmp(env[i], cmd, length) == 0)
	// 	{
	// 		len = path_index(env[i]);
	// 		cmd = ft_substr(env[i], len, 1000);
	// 		// printf("the string is : %s\n", cmd);
	// 		// printf("ha\n");
	// 		break ;
	// 	}
	// 	i++;
	// }
	return (NULL);
}
//==================================================================================================================

int	dollar_index(char *cmd)
{
	int	count;

	count = 0;
	while (*cmd)
	{
		if (*cmd == '$')
			break ;
		cmd++;
		count++;
	}
	return (count);
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
	int i;
	// (void) str;

	if (ft_strcmp(str, "") == 0)
	{
		return (NULL);
	}
	tmp = env;
	i = 0;
	while (tmp)
	{
		if (ft_strncmp(str, tmp->content, var_length(tmp->content)) == 0 && (var_length(tmp->content) == ft_strlen(str)))
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

static char	*ft_expand(char *cmd, t_list *env, int *i)
{
	int		limiter;
	static char *full_str;
	char	*lineup;
	char	*str;
	char	*value;

	limiter = *i + 1;
	while (cmd[limiter] && (ft_isalnum(cmd[limiter]) || ft_ischar(cmd[limiter])))
	{
		limiter++;
	}
	str = ft_substr(cmd, (*i + 1), limiter - (*i + 1));
	value = env_value(str, env);
	if (value)
	{
		lineup = ft_substr(cmd, 0, (*i));
		full_str = ft_strjoin(lineup, value);
		// free(value);
	}
	else
	{
		lineup = ft_substr(cmd, 0, (*i));
		full_str = ft_strdup(lineup);
	}
	free(lineup);
	if (cmd[limiter])
	{
		if (cmd[limiter - 1] == '$')
			{
				if (cmd[limiter] == '"' || cmd[limiter] == '\'')
					lineup = ft_substr(cmd, limiter, 1000);
				else
					lineup = ft_substr(cmd, limiter - 1, 1000);
		
			}
		else
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
	*i = ft_strlen(full_str);
	free(full_str);
	free(str);
	if ((*i > 0) && (cmd[*i] == '$' || cmd[*i] == '\'' || cmd[*i] == '"'))
            *i -= 1;
	return(cmd);
}

static char	*ft_expand_exit(char *cmd, t_list *env, int *i)
{
	(void) env;
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
	int	i;
	int		flag;

	i = 0;
	flag = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
		{
			flag = is_outside(flag, cmd[i]);
		}
		if (cmd[i] && cmd[i + 1] && cmd[i] == '$' && cmd[i + 1] == '$' && flag != 1)
			i++;
		if (cmd[i] && cmd[i + 1] && cmd[i] == '$' && cmd[i + 1] == '?' && flag != 1)
		{
			cmd = ft_expand_exit(cmd, env, (&i));
		}
		if (cmd[i] && cmd[i + 1] && cmd[i] == '$' && cmd[i + 1] != '?' && flag != 1)
		{
			cmd = ft_expand(cmd, env, (&i));
		}
		if (cmd[i] && cmd[i + 1] && cmd[i] == '$' && cmd[i + 1] == '?' && flag != 1)
		{
			cmd = ft_expand_exit(cmd, env, (&i));
		}
		if (cmd[i])
			i++;
	}
	return (cmd);
}

char	**expander(char **cmd, t_list *env, int *arr)
{
	int		i;
	int		j;
	char	**new_cmd;
	int		count = 0;

	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (ft_strchr(cmd[i], '$') && arr[i] != HEREDOC_LIM)
		{
			cmd[i] = expand_processor(cmd[i], env);
			printf("this is the str (%s)\n", cmd[i]);
		}
		i++;
	}
	while (cmd[j])
	{
		if (ft_strcmp(cmd[j], "") != 0)
			count++;
		j++;
	}
	new_cmd = (char **)ft_calloc(sizeof(char *), count + 1);
	new_cmd[count] = NULL;
	j = 0;
	i = 0;
	while (cmd[j])
	{
		if (ft_strcmp(cmd[j], "") != 0)
		{
			new_cmd[i] = ft_strdup(cmd[j]);
			i++;
		}
		j++;
	}
	// free_2d(cmd);
	return (new_cmd);
}
