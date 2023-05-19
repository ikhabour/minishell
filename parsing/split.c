/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 22:23:04 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/01 19:11:31 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_outside(int flag, char c)
{
	if (c == '\'')
	{
		if (flag == 0)
			flag = 1;
		else if (flag == 1)
			flag = 0;
	}
	else if (c == '"')
	{
		if (flag == 2)
			flag = 0;
		else if (flag == 0)
			flag = 2;
	}
	return (flag);
}

int	calc_words(char *s, char sep)
{
	int	i;
	int	flag;
	int	word;

	i = 0;
	flag = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			flag = is_outside(flag, s[i]);
		if (s[i] == sep && flag == 0)
		{
			if (s[i] && s[i] == sep)
			{
				word++;
				// i++;
			}
		}
		if (!s[i + 1])
			word++;
		i++;
	}
	return (word);
}

int	ft_wordlen(char *s, int i, char c)
{
	int	len;
	int	flag;

	len = 0;
	flag = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			flag = is_outside(flag, s[i]);
		}
		if (s[i] == c && flag == 0)
			break;
		i++;
		len++;
	}
	return (len);
}

char	*ft_charge(char *s, int i, char c, int flag)
{
	char	*str;
	int		len;
	int		j;

	len = ft_wordlen(s, i, c) + 1;
	str = (char *)malloc(sizeof(char) * len);
	if (!str)
		return (NULL);
	j = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			flag = is_outside(flag, s[i]);
		}
		if (s[i] == c && flag == 0)
			break ;
		str[j] = s[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (str);
}

// void	*freeini(char **string, int len)
// {
// 	int	i;

// 	i = 0;
// 	while (i < len)
// 	{
// 		free(string[i]);
// 		i++;
// 	}
// 	free(string);
// 	return (NULL);
// }

char	**ft_split(char *s, char sep)
{
	char	**strs;
	int		j = 0;
	int		length = 0;
	int		words;

	if (!s)
		return (0);
	words = calc_words(s, sep);
	strs = (char **)ft_calloc(sizeof(char *), words + 1);
	if (!strs || !s)
		return (NULL);
	strs[words] = NULL;
	while (j < words)
	{
		length = ft_wordlen(s, 0, sep);
		// printf("string: %s---> %d------> %d\n", s, j, length);
		strs[j] = ft_substr(s, 0, length);
		// printf("str: (%s), the length: %d\n", strs[j],length);
		s += length;
		if (*s == sep)
			s++;
		j++;
	}
	// printf("this is the count: %d\n", calc_words(s, ' '));
	// printf("idk what the fuck m doing: (%s)\n", s);
	// printf("the len of the first word : %d\n", ft_wordlen(s, 0, ' '));
	return (strs);
}