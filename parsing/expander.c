/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:14:21 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/03 23:28:34 by bhazzout         ###   ########.fr       */
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

// int	dollar_index(char *cmd, int i)
// {
// 	while (i > 0)
// 	{
// 		if (cmd[i] == '$')
// 			break ;
// 		i--;
// 	}
// 	// if (i == 0)
// 	return (i);
// }

// void	expand_processor(char *cmd, char **env)
// {
// 	int		i;
// 	char	*str;
// 	char	*str2;
// 	int		prev_index;
// 	// char	*value;
// 	char	*substring;
// 	int		flag = 0;
// 	char	*full_str;
// 	// char	*full_str;

// 	i = 0;
// 	while (cmd[i])
// 	{
// 		if (cmd[i] == '$')
// 		{
// 			if (flag == 1)
// 			{
// 				prev_index = dollar_index(cmd, i - 1);
// 				str = ft_substr(cmd, prev_index, (i - prev_index));
// 				str = get_value(++str, env);
// 				// printf("this is the full str: %s\n", str);
// 			}
// 			else if (flag == 0)
// 			{
// 				substring = ft_substr(cmd, 0, i);
// 				flag = 1;
// 			}
// 			full_str = ft_strjoin(substring, str);
// 			// if (get_value(str, env) != NULL)
// 			// 	value = get_value(str, env);
// 			str2 = ft_substr(cmd, i, 1000);
// 			// printf("str from :%d to %d is===>(%s)\n", prev_index, i - prev_index, str);
// 			// printf("when str2 is: (%s), the first str is: (%s), the substring is: (%s)\n", str2, str, substring);
			
// 		}
// 		i++;
// 	}
// 	// printf("this is the full str: %s\n", str2);
// 	printf("this is the full str: %s\n", full_str);
// }

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

char	*env_value(char *str, t_list *env)
{
	t_list	*tmp;
	char	*value;
	int i;
	// (void) str;

	tmp = env;
	i = 0;
	while (tmp)
	{
		// printf("(%s)\n", tmp->content);
		if (ft_strncmp(str, tmp->content, ft_strlen(str)) == 0)
		{
			value = ft_strdup(tmp->content);
			while (value[i] != '=')
				i++;
			// printf("char : %c\n", value[i]);
			// value = tmp->env_value;./
			return (tmp->content + i + 1);
		}
		// printf("the name is : %s\n", tmp->env_name);
		// printf("the value is : %s\n", tmp->env_value);
		tmp = tmp->next;
	}
	return (NULL);
}

static char	*ft_expand(char *cmd, t_list *env, int *i)
{
	int		limiter;
	static char *full_str;
	char	*lineup;
	char	*str;
	char	*value;

	limiter = *i + 1;
	while (cmd[limiter] && ft_isalnum(cmd[limiter]))
	{
		// *i++;
		limiter++;
	}
	str = ft_substr(cmd, (*i + 1), limiter - (*i + 1));
	value = env_value(str, env);
	printf("this is the value: (%s)\n", value);
	lineup = ft_substr(cmd, 0, (*i));
	if (value)
		full_str = ft_strjoin(lineup, value);
	else
		full_str = ft_strjoin(lineup, "");
	free(lineup);
	if (cmd[limiter])
		lineup = ft_substr(cmd, limiter, 1000);
	else
	{
		limiter -= 1;
		lineup = ft_strdup(" ");
	}
	free(cmd);
	cmd = ft_strjoin(full_str, lineup);
	*i = ft_strlen(full_str);
	free(full_str);
	free(lineup);
	free(str);
	if (cmd[*i] == '$' || cmd[*i] == '"' || cmd[*i] == '\'')
            *i -= 1;
    else if (!cmd[*i])
            *i = -2;
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
	printf("before : (%s) value : (%s) after : (%s)\n", lineup, value, str);
	free(lineup);
	free(cmd);
	cmd = ft_strjoin(full_str, str);
	*i = ft_strlen(full_str);
	printf("this is the full_str : (%s)\n", full_str);
	free(full_str);
	free(value);
	return (cmd);
}

char	*expand_processor(char *cmd, t_list *env)
{
	int	i;
	// char	*str;
	// char	*str2;
	int		flag;
	(void) env;

	i = 0;
	flag = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\'' || cmd[i] == '"')
		{
			// printf("haaa\n");
			flag = is_outside(flag, cmd[i]);
		}
		if (cmd[i + 1] && cmd[i] == '$' && cmd[i + 1] == '?' && flag != 1)
		{
			cmd = ft_expand_exit(cmd, env, (&i));
			printf("str: ====(%s)\n", cmd);
		}
		// printf("(%c)===(%d)\n", cmd[i], flag);
		if (cmd[i + 1] && cmd[i] == '$' && cmd[i + 1] != '?' && flag != 1)
		{
			cmd = ft_expand(cmd, env, (&i));
		}
		if (cmd[i])
			i++;
	}
	return (cmd);
}

void	expander(char **cmd, t_list *env)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd[i])
	{
		if (ft_strchr(cmd[i], '$'))
		{
			cmd[i] = expand_processor(cmd[i], env);
		}
		// j = 0;
		// while (cmd[i][j])
		// {
		// 	if (cmd[i][j] == ' ' || (cmd[i][j] >= 9 && cmd[i][j] <= 13))
		// 		break;
		// 	j++;
		// }
		i++;
	}
}