/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:36:26 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/31 22:41:58 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_s = 0;

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
	printf("this is the length : %d\n", len);
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

	cmd_token = ft_calloc(sizeof (int) , elements + 1);
	if (!cmd_token)
		return (NULL);
	i = 0;
	while (cmd_array[i])
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

int	is_space(char *str)
{
	int i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (!str[i])
		return (1);
	return (0);
}

void	free_2d(char **array)
{
	int i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	free_all(char *input, char **array)
{
	free(input);
	free_2d(array);
}

void	get_input(char *input, t_list **env)
{
	int		len;
	char	**cmd_array;
	t_list	*commands;
	char	*history;
	int		*arr;
	(void) env;

	input = readline("Minishell> ");
	history = input;
	if (!input || ft_strcmp(input, "") == 0)
	{
		free(input);
		return ;
	}
	if (is_space(input))
	{
		add_history(input);
		free(input);
		return ;
	}
	len = get_length(input);
	// printf("len is : %d\n", len);
	check_line(input);
	input = fill_line(input, len);
	input = add_spaces(input);
	printf("this is the line : %s\n", input);
	cmd_array = ft_split(input, ' ');
	split_print(cmd_array);
	arr = array_tokens(cmd_array, num_elemnts(cmd_array));
	array_printer(arr);
	if (op_order(arr))
	{
		free(input);
		// free_2d(cmd_array);
		return ;
	}
	expander(cmd_array, *env);
	cmd_array = quote_delete(cmd_array);
	commands = list_cmds(cmd_array, arr);
	// print_list(commands);
	// add_history(history);
	// if (ft_lstsize(commands) > 1)
	// {
	// 	if (ft_lstsize(commands) == 2)
	// 		execute_pipe_commands(commands, env_to_array(env), cmd_array);
	// 	else
	// 		multiple_pipes(commands, env);
	// 	free_all(input, cmd_array);
	// 	return ;
	// }
	// if (execute_builtins(commands, env))
	// {
	// 	free(input);
	// 	free_2d(cmd_array);
	// 	return ;
	// }
	// execute_commands(commands, env, cmd_array);
	// free_2d(cmd_array);
	free(input);
}

int main (int ac, char **av, char **envp)
{
	
	char    input;
	t_list *env;
	(void)  ac;
	(void)  av;

	// (void)  envp;
	env = make_env(envp);
	while (1)
	{
		get_input(&input, &env);
	}
	get_env(envp);
}