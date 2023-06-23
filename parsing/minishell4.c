/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:51:43 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:51:52 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*fill_line(char *input)
{
	char	*tmp;

	tmp = ft_strtrim(input, " ");
	free(input);
	return (tmp);
}

int	num_elemnts(char **cmd_array)
{
	int	i;

	i = 0;
	while (cmd_array[i])
		i++;
	return (i);
}

int	name_arg(int *cmd_token, char **cmd_array, int i, int *flag)
{
	cmd_token[i] = CMD_NAME;
	if ((i > 0) && (cmd_token[i - 1] == CMD_NAME || cmd_token[i - 1] == CMD_ARG)
		&& ft_strcmp(cmd_array[i], ">") && ft_strcmp(cmd_array[i], "<")
		&& ft_strcmp(cmd_array[i], ">>") && ft_strcmp(cmd_array[i], "<<")
		&& ft_strcmp(cmd_array[i], "|"))
		cmd_token[i] = CMD_ARG;
	else if ((i > 0) && *flag == 1 && (cmd_token[i - 1] == R_OUT_FILE
			|| cmd_token[i - 1] == R_IN_FILE || cmd_token[i - 1] == R_APP_FILE
			|| cmd_token[i - 1] == CMD_ARG) && ft_strcmp(cmd_array[i], ">")
		&& ft_strcmp(cmd_array[i], "<") && ft_strcmp(cmd_array[i], ">>")
		&& ft_strcmp(cmd_array[i], "<<") && ft_strcmp(cmd_array[i], "|"))
		cmd_token[i] = CMD_ARG;
	*flag = 1;
	return (cmd_token[i]);
}

int	enum_token(int *cmd_token, char **cmd_array, int i, int *flag)
{
	if (ft_strcmp(cmd_array[i], "|") == 0)
		cmd_token[i] = PIPE;
	else if (cmd_array[i + 1] && (ft_strcmp(cmd_array[i], "<>") == 0))
		cmd_token[i] = R_IN_OUT;
	else if (ft_strcmp(cmd_array[i], "<") == 0)
		cmd_token[i] = R_IN_SIG;
	else if (ft_strcmp(cmd_array[i], ">") == 0)
		cmd_token[i] = R_OUT_SIG;
	else if (ft_strcmp(cmd_array[i], ">>") == 0)
		cmd_token[i] = R_APP_SIG;
	else if (ft_strcmp(cmd_array[i], "<<") == 0)
		cmd_token[i] = HEREDOC_SIG;
	else if ((i > 0) && (ft_strcmp(cmd_array[i - 1], "<<") == 0))
		cmd_token[i] = HEREDOC_LIM;
	else if ((i > 0) && (ft_strcmp(cmd_array[i - 1], ">>") == 0))
		cmd_token[i] = R_APP_FILE;
	else if ((i > 0) && (ft_strcmp(cmd_array[i - 1], ">") == 0))
		cmd_token[i] = R_OUT_FILE;
	else if ((i > 0) && (ft_strcmp(cmd_array[i - 1], "<>") == 0))
		cmd_token[i] = R_OUT_FILE;
	else if ((i > 0) && (ft_strcmp(cmd_array[i - 1], "<") == 0))
		cmd_token[i] = R_IN_FILE;
	else
		cmd_token[i] = name_arg(cmd_token, cmd_array, i, flag);
	return (cmd_token[i]);
}

int	*array_tokens(char **cmd_array, int elements)
{
	int	*cmd_token;
	int	i;
	int	flag;

	flag = 0;
	cmd_token = ft_calloc(sizeof(int), elements + 1);
	if (!cmd_token)
		return (NULL);
	i = 0;
	while (cmd_array[i])
	{
		cmd_token[i] = enum_token(cmd_token, cmd_array, i, &flag);
		i++;
	}
	return (cmd_token);
}
