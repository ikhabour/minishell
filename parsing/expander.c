/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 14:14:21 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/09 16:19:42 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	expand_processor(char *cmd, char **env)
{
	int	i;
	// int	index;
	char	*str;
	char	*str2;
	// char	*path;
	// (void) env;

	i = 0;
	while (cmd[i])
	{
		// index = dollar_index(cmd);
		// str = ft_substr(cmd, 0, index);
		if (cmd[i] == '$')
		{
			str = ft_substr(cmd, 0, i);
			str2 = ft_substr(cmd, i, 1000);
			str = get_value(++str, env);
			printf("str: %s\n", str);
			printf("str2: %s\n", str2);
		}
		i++;
	}
}

void	expander(char **cmd, char **env)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_strchr(cmd[i], '$'))
		{
			expand_processor(cmd[i], env);
		}
		i++;
	}
}