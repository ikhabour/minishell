/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_processor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 00:27:03 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/18 20:27:56 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strtrim(char *s1, char *set)
{
	int		i;
	size_t	s1len;

	i = 0;
	if (!s1 || !set)
		return (0);
	if (s1[i] == '\0')
		return (ft_strdup(""));
	if (set[i] == '\0')
		return (ft_strdup(s1));
	while (s1[i] && ft_strchr(set, s1[i]))
		i++;
	s1len = ft_strlen(s1) - 1;
	while (s1[i] >= 0 && ft_strchr(set, s1[s1len]))
		s1len--;
	return (ft_substr(s1, i, (s1len - i + 1)));
}

int	space_before(char *input, int i)
{
	if (i == 0)
		return (1);
	if ((i > 0) && (input[i - 1] == ' '))
		return (1);
	return (0);
}

int	space_after(char *input, int i)
{
	if (input[i + 1] && input[i + 1] == ' ')
		return (1);
	return (0);
}

int	full_length(char *input)
{
	int	i;
	int	count;
	int	flag;

	i = 0;
	count = 0;
	while (input[i])
	{
		flag = 0;
		if (input[i] == '|' || input[i] == '>' || input[i] == '<')
		{
			if (space_before(input, i) == 0)
				count++;
			if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
				i++;
			if (space_after(input, i) == 0)
			{
				count++;
				// flag = 1;
			}
			// if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
			// 	count--;
		}
		count++;
		i++;
	}
	return (count);
}

void	*add_space_before(char *input, int i)
{
	char	*str;
	char	*str1;
	char	*str2;
	// char	*full_str;

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
	// printf("this is the str: %s\n", str);
	// printf("this is the str1: %s\n", str1);
	str2 = ft_strjoin(str, " ");
	free(input);
	input = ft_strjoin(str2, str1);
	free(str);
	free(str1);
	free(str2);
	return (input);
}

char	*add_spaces(char *input)
{
	int 	i;
	int	flag = 0;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			flag = is_outside(flag, input[i]);
		}
		if ((input[i] == '|' || input[i] == '>' || input[i] == '<') && flag == 0)
		{
			if (space_before(input, i) == 0)
				input = add_space_before(input, i);
			if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<') || (input[i] == '<' && input[i + 1] == '>'))
				i++;
		}
		i++;
	}
	// printf("String before: (%s)\n", input);
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			flag = is_outside(flag, input[i]);
		}
		if ((input[i] == '|' || input[i] == '<' || input[i] == '>') && flag == 0)
		{
			if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<') || (input[i] == '<' && input[i + 1] == '>'))
				i++;
			if (space_after(input, i) == 0)
				input = add_space_after(input, i);
		}
		i++;
	}
	// printf("String after: (%s)\n", input);
	return (input);
}

char	*skip_spaces(char *input)
{
	int		i;
	int		count;
	// char	*tmp = NULL;

	i = 0;
	count = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	int j = i;
	while (input[i])
	{
		count++;
		i++;
	}
	while ((i > 0) && (input[i - 1] == ' ' || input[i - 1] == '\t'))
	{
		count--;
		i--;
	}
	i = 0;
	while (j < count)
	{
		// tmp[i];
		j++;
	}
	// input[i] = '\0';
	return (input);
}
