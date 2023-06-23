/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_char_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:17:40 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 02:03:04 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	char_checker(char *str, int flag, int i, char *input)
{
	if (str[i] == '\'' || str[i] == '"')
		flag = is_outside(flag, str[i]);
	if ((str[i] == ';' || str[i] == '\\' || str[i] == '&') && flag == 0)
	{
		write(2, "Syntax error, unexpected token.\n", 32);
		g_sigs.exit_s = 1;
		free(input);
		free(str);
		return (1);
	}
	return (0);
}

int	general_check(char *str, char *input)
{
	if (check_quotes(str))
	{
		free(input);
		free(str);
		return (1);
	}
	if (check_pipe(str))
	{
		free(input);
		free(str);
		return (1);
	}
	if (check_redirec_op(str))
	{
		free(input);
		free(str);
		return (1);
	}
	return (0);
}

int	check_line(char *input)
{
	int		i;
	int		flag;
	char	*str;

	i = 0;
	flag = 0;
	str = ft_strtrim(input, " ");
	while (str[i])
	{
		if (char_checker(str, flag, i, input))
			return (1);
		i++;
	}
	if (general_check(str, input))
		return (1);
	free(str);
	return (0);
}
