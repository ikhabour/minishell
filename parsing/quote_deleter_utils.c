/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_deleter_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 03:56:09 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 04:02:21 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	d_quoted(char *str, char *cmd, int *i, int *j)
{
	int	i_dex;
	int	j_dex;

	i_dex = *i;
	j_dex = *j;
	i_dex += 1;
	while (str[i_dex] && str[i_dex] != '"')
	{
		cmd[j_dex] = str[i_dex];
		j_dex++;
		i_dex++;
	}
	*i = i_dex;
	*j = j_dex;
}

void	s_quoted(char *str, char *cmd, int *i, int *j)
{
	int	i_dex;
	int	j_dex;

	i_dex = *i;
	j_dex = *j;
	i_dex += 1;
	while (str[i_dex] && str[i_dex] != '\'')
	{
		cmd[j_dex] = str[i_dex];
		j_dex++;
		i_dex++;
	}
	*i = i_dex;
	*j = j_dex;
}
