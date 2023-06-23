/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_processor1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:49:08 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:54:18 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*add_space_before(char *input, int i)
{
	char	*str;
	char	*str1;
	char	*str2;

	str = ft_substr(input, 0, i);
	str1 = ft_substr(input, i, 1000);
	str2 = ft_strjoin(str, " ");
	free(input);
	input = ft_strjoin(str2, str1);
	free(str);
	free(str1);
	free(str2);
	return (input);
}

void	*add_space_after(char *input, int i)
{
	char	*str;
	char	*str1;
	char	*str2;

	str = ft_substr(input, 0, i + 1);
	str1 = ft_substr(input, i + 1, 1000);
	str2 = ft_strjoin(str, " ");
	free(input);
	input = ft_strjoin(str2, str1);
	free(str);
	free(str1);
	free(str2);
	return (input);
}

char	*process_before(char *input, int i, int *flag)
{
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
			*flag = is_outside(*flag, input[i]);
		if ((input[i] == '|' || input[i] == '>' || input[i] == '<')
			&& *flag == 0)
		{
			if (space_before(input, i) == 0)
				input = add_space_before(input, i);
			if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<'
					&& input[i + 1] == '<') || (input[i] == '<' && input[i
						+ 1] == '>'))
				i++;
		}
		i++;
	}
	return (input);
}

char	*process_after(char *input, int i, int *flag)
{
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
			*flag = is_outside(*flag, input[i]);
		if ((input[i] == '|' || input[i] == '<' || input[i] == '>')
			&& *flag == 0)
		{
			if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<'
					&& input[i + 1] == '<') || (input[i] == '<' && input[i
						+ 1] == '>'))
				i++;
			if (space_after(input, i) == 0)
				input = add_space_after(input, i);
		}
		i++;
	}
	return (input);
}

char	*add_spaces(char *input)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	input = process_before(input, i, &flag);
	i = 0;
	input = process_after(input, i, &flag);
	return (input);
}
