/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_deleter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 15:32:20 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/24 23:37:11 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

// char	*quote_processor(char *str)
// {
// 	int	i;
// 	int	j = 0;
// 	int	length = 0;
// 	int	flag = 0;
// 	char	*cmd;

// 	i = 0;
// 	length = no_quotes_len(str);
// 	printf("the length is : %d\n", length);
// 	cmd = malloc (length + 1);
// 	while (str[i])
// 	{
// 		while (str[i] && str[i + 1] && ((str[i] == '\'' && str[i + 1] == '\'') || (str[i] == '"' && str[i + 1] == '"')))
// 			i++;
// 		if ((str[i] == '\'' || str[i] == '"') && flag == 0)
// 		{
// 			flag = is_outside(flag, str[i]);
// 			i++;
// 		}
// 		if (str[i] && (str[i] == '\'' || str[i] =='"'))
// 		{
// 			flag = is_outside(flag, str[i]);
// 			// if (str[i] == str[i + 1])
// 			// 	i++;
// 		}
// 		if ((str[i] == '\'' || str[i] == '"') && flag == 0)
// 		{
// 			flag = is_outside(flag, str[i]);
// 			i++;
// 		}
// 		if (str[i] != '\0')
// 		{
// 			// printf("(%d) === (%c)\n", j, str[i]);
// 			cmd[j] = str[i];
// 			j++;
// 			i++;
// 		}
// 	}
// 	cmd[j] = '\0';
// 	free(str);
// 	return (cmd);
// }

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
		if (str[i] == '\'' && flag == 0)
		{
			printf("haaaa\n");
			i++;
			flag = 1;
		}
		if (flag == 1 && str[i] == '\'')
		{
			printf("haaaa\n");
			i++;
			flag = 0;
		}
		if (str[i] == '"' && flag == 0)
		{
			printf("haaaa\n");
			i++;
			flag = 2;
		}
		if (flag == 2 && str[i] == '"')
		{
			printf("haaaa\n");
			i++;
			flag = 0;
		}
		if (str[i])
		{
			cmd[j] = str[i];
			j++;
			i++;
		}
	}
	cmd[j] = '\0';
	printf("this is the str: (%s)\n", cmd);
	return (NULL);
}

char	**quote_delete(char **cmd)
{
	int		i;
	// char	*str;

	i = 0;
	while (cmd[i])
	{
		// printf("This is the str: (%s)\n", cmd[i]);
		cmd[i] = quote_processor(cmd[i]);
		i++;
	}
	return (cmd);
}