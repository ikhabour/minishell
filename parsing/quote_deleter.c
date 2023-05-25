/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_deleter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:32:20 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/25 21:50:10 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	quoted(char *str, char *cmd, char c, int *i, int *j)
{
	int	i_dex = *i;
	int	j_dex = *j;
	i_dex += 1;
	while (str[i_dex] && str[i_dex] != c)
	{
		cmd[j_dex] = str[i_dex];
		j_dex++;
		i_dex++;
	}
	*i = i_dex;
	*j = j_dex;
}

char	*quote_processor(char *str)
{
	int		i;
	int		length;
	char	*cmd;
	int		j;
	int		flag;

	i = 0;
	j = 0;
	flag = 0;
	length = no_quotes_len(str);
	cmd = malloc (length + 1);
	if (!cmd)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '"')
			quoted(str, cmd, '"', &i, &j);
		else if (str[i] == '\'')
			quoted(str, cmd, '\'', &i, &j);
		else
		{
			cmd[j] = str[i];
			j++;
		}
		i++;
	}
	cmd[j] = '\0';
	return (cmd);
}

char	**quote_delete(char **cmd)
{
	int		i;

	i = 0;
	while (cmd[i])
	{
		cmd[i] = quote_processor(cmd[i]);
		printf("this is the finale result: (%s)\n", cmd[i]);
		i++;
	}
	return (cmd);
}