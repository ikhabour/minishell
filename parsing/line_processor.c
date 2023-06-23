/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_processor.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 00:27:03 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 02:49:03 by ikhabour         ###   ########.fr       */
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
			if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<'
					&& input[i + 1] == '<'))
				i++;
			if (space_after(input, i) == 0)
			{
				count++;
			}
		}
		count++;
		i++;
	}
	return (count);
}
