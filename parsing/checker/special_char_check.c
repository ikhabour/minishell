/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_char_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 09:17:40 by bhazzout          #+#    #+#             */
/*   Updated: 2023/04/27 10:17:38 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_line(char *input)
{
	input = skip_spaces(input);
	if (ft_strchr(input, '\\') || ft_strchr(input, ';') || ft_strchr(input, '&')) //special characters
	{
		printf("Error, special character.\n");
		return (1);
	}
	if (check_quotes(input))// check if the command line contains an unclosed quote
	{
		// printf("Error, unclosed quote.\n");
		return (1);
	}
	if (check_pipe(input))
		return (1);
	if (check_redirec_op(input))
		return (1);
	return (0);
}
