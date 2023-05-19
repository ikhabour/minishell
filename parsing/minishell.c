/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:36:26 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/17 16:55:32 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_length(char *input)
{
	int	i;
	int	len;
	int	flag = 0;

	i = 0;
	len = 0;
	while (input[i] && (input[i] == ' '|| input[i] == '\t'))
		i++;
	while (input[i])
	{
		// skip_quotes(input, i)
		if (input[i] == '"' || input[i] =='\'')
			flag = is_outside(flag, input[i]);
		if ((input[i] == ' ' || input[i] == '\t') && flag == 0)
		{
			while (input[i] && (input[i] == ' ' || input[i] == '\t'))
				i++;
			i--;
		}
		len++;
		i++;
	}
	i--;
	if (input[i] == ' ' || input[i] == '\t')
	{
		i--;
		len--;
	}
	return (len);
}


char	*fill_line(char *input, int len)//get the line each of it's word separated by one space
{
	int		i;
	int		j;
	char	*line;
	int		flag = 0;

	i = 0;
	j = 0;
	line = malloc (len + 1);
	input = skip_spaces(input);
	while (input[i])
	{
		if (input[i] == '"' || input[i] =='\'')
			flag = is_outside(flag, input[i]);
		if ((input[i] == ' ' || input[i] == '\t') && flag == 0)
		{
			while (input[i] == ' ' || input[i] == '\t')
				i++;
			i--;
		}
		line[j] = input[i];
		j++;
		i++;
	}
	line[j] = '\0';
	// printf("this is the line-->%s\n", line);
	return (line);
}

void	array_printer(int *input)
{
	int i;

	i = 0;
	while (input[i])
	{
		printf("%d\n", input[i]);
		i++;
	}
}

int	num_elemnts(char **cmd_array)
{
	int	i;

	i = 0;
	while (cmd_array[i])
	{
		i++;
	}
	return (i);
}

int	*array_tokens(char **cmd_array, int elements)
{
	int	*cmd_token;
	int	i;

	// printf("this is the elements : %d\n", elements);
	cmd_token = ft_calloc(sizeof (int) , elements + 1);
	if (!cmd_token)
		return (NULL);
	i = 0;
	while (cmd_array[i])
	{
		if (ft_strcmp(cmd_array[i], "|") == 0)
			cmd_token[i] = PIPE;
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
		else if ((i > 0) && (ft_strcmp(cmd_array[i - 1], "<") == 0))
			cmd_token[i] = R_IN_FILE;
		else
			cmd_token[i] = CMD_NAME;
		if ((i > 0) && (cmd_token[i - 1] == CMD_NAME || cmd_token[i - 1] == CMD_ARG) && 
			ft_strcmp(cmd_array[i], ">") && ft_strcmp(cmd_array[i], "<") && 
			ft_strcmp(cmd_array[i], ">>") && ft_strcmp(cmd_array[i], "<<") && 
			ft_strcmp(cmd_array[i], "|"))
			cmd_token[i] = CMD_ARG;
		i++;
	}
	return (cmd_token);
}

void	get_input(char *input, char **env)
{
	int		len;
	char	**cmd_array;
	t_list	**commands;
	int		*arr;
	(void) env;

	input = readline("Minishell: ");
	if (ft_strcmp(input, "") == 0)
		return ;
	len = get_length(input);
	// printf("len is : %d\n", len);
	check_line(input);
	input = fill_line(input, len);
	// printf("this is the line : %s\n", input);
	input = add_spaces(input);
	cmd_array = ft_split(input, ' ');
	// split_print(cmd_array);
	arr = array_tokens(cmd_array, num_elemnts(cmd_array));
	op_order(arr);
	cmd_array = quote_delete(cmd_array);
	// expander(cmd_array, env);
	// split_print(cmd_array);
	// printf("======================\n");
	// array_printer(arr);
	commands = list_cmds(cmd_array, arr);
	add_history(input);
	free (input);
}

int main (int ac, char **av, char **env)
{
	
	char    input;
	(void)  ac;
	(void)  av;
	// (void)  env;
	while (1)
	{
		get_input(&input, env);
	}
	get_env(env);
}