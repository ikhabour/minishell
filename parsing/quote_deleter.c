/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_deleter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 10:38:19 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/05 14:09:53 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sub_lenght(char *str, int i, char c)
{
	(void) c;
	while (str[i])
	{
		
		i++;
	}
	return (0);
}

int	no_quotes_len(char *str)
{
	int	i;
	int	flag = 0;
	int	count = 0;
	int	j = 0;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			flag = is_outside(flag, str[i]);
		if ((str[i] == '\'' || str[i] == '"') && flag == 0)
		{
			count--;
			j++;
		}
		count++;
		i++;
	}
	count -= j;
	return (count);
}

char	*quote_processor(char *str)
{
	int	i;
	int	j = 0;
	int	length = 0;
	int	flag = 0;
	char	*cmd;

	i = 0;
	length = no_quotes_len(str);
	// printf("the length is : %d\n", length);
	cmd = malloc (length + 1);
	while (str[i])
	{
		while (str[i] && str[i + 1] && ((str[i] == '\'' && str[i + 1] == '\'') || (str[i] == '"' && str[i + 1] == '"')))
			i++;
		if ((str[i] == '\'' || str[i] == '"') && flag == 0)
		{
			flag = is_outside(flag, str[i]);
			i++;
		}
		if (str[i] && (str[i] == '\'' || str[i] =='"'))
		{
			flag = is_outside(flag, str[i]);
			// if (str[i] == str[i + 1])
			// 	i++;
		}
		if ((str[i] == '\'' || str[i] == '"') && flag == 0)
		{
			flag = is_outside(flag, str[i]);
			i++;
		}
		if (str[i] != '\0')
			cmd[j++] = str[i++];
	}
	cmd[j] = '\0';
	free(str);
	return (cmd);
}

char	**quote_delete(char **cmd)
{
	int		i;
	// char	*str;

	i = 0;
	while (cmd[i])
	{
		cmd[i] = quote_processor(cmd[i]);
		// printf("This is the str: (%s)\n", str);
		i++;
	}
	return (cmd);
}