/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_deleter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:32:20 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 04:03:22 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_quotes_len(char *str)
{
	int	i;
	int	flag;
	int	count;
	int	j;

	flag = 0;
	count = 0;
	j = 0;
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

char	*no_quote_fill(char *str, char *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"')
			d_quoted(str, cmd, &i, &j);
		else if (str[i] == '\'')
			s_quoted(str, cmd, &i, &j);
		else
		{
			cmd[j] = str[i];
			j++;
		}
		i++;
	}
	free(str);
	cmd[j] = '\0';
	return (cmd);
}

char	*quote_processor(char *str)
{
	int		length;
	char	*cmd;
	int		flag;

	flag = 0;
	length = no_quotes_len(str);
	cmd = malloc (length + 1);
	if (!cmd)
		return (NULL);
	cmd = no_quote_fill(str, cmd);
	return (cmd);
}

int	check_delimiter(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '"' || cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

char	**quote_delete(char **cmd, int *delimiter, int *arr)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		if (arr[i] == HEREDOC_LIM)
		{
			*delimiter = check_delimiter(cmd[i]);
		}
		cmd[i] = quote_processor(cmd[i]);
		i++;
	}
	return (cmd);
}
