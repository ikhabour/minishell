/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:36:26 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 05:46:01 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_s = 0;
int return_val = 0;

char	*fill_line(char *input)//get the line each of it's word separated by one space
{
	char *tmp;

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
		|| cmd_token[i - 1] == R_IN_FILE || cmd_token[i - 1] == R_APP_FILE ||
		cmd_token[i - 1] == CMD_ARG) && ft_strcmp(cmd_array[i], ">")
		&& ft_strcmp(cmd_array[i], "<")
		&& ft_strcmp(cmd_array[i], ">>") && ft_strcmp(cmd_array[i], "<<")
		&& ft_strcmp(cmd_array[i], "|"))
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
	int	flag = 0;

	cmd_token = ft_calloc(sizeof (int) , elements + 1);
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

int	is_space(char *str)
{
	int i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (!str[i])
		return (1);
	return (0);
}


void	free_files(t_list *files)
{
	t_filetype *file;
	t_list *tmp;

	file = (t_filetype *)files->content;
	while (files)
	{
		free(file->file_name);
		free(file->type);
		free(file->red);
		tmp = files;
		files = files->next;
		free(tmp);
		free(file);
		if (files)
			file = (t_filetype *)files->content;
	}
}

void	my_free(t_list *commands)
{
	t_cmds *ptr;
	t_list *tmp;

	ptr = (t_cmds *)commands->content;
	while (commands)
	{
		free(ptr->cmd_name);
		if (ptr->option)
			free_2d(ptr->option);
		if (ptr->files)
			free_files(ptr->files);
		tmp = commands;
		commands = commands->next;
		free(tmp);
		free(ptr);
		if (commands)
			ptr = (t_cmds *)commands->content;
	}
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

int	valid_command(char **new)
{
	int	i;
	int	j;
	int	flag;

	i = 0;
	j = 0;
	flag = 0;
	if (new[i] == NULL)
		return (1);
	while (new[i])
		i++;
	if (ft_strcmp(new[i - 1], "|") == 0)
		return (1);
	if (ft_strcmp(new[i - 1], ">") == 0)
	{
		printf("Minishell: ambiguous redirect\n");
		return (1);
	}
	if (ft_strcmp(new[i - 1], ">>") == 0)
	{
		printf("Minishell: ambiguous redirect\n");
		return (1);
	}
	return (0);
}

char	*signal_handler(char *input)
{
	struct termios	term;
	struct termios	original;

	sig_handler();
	tcgetattr(0, &term);
	tcgetattr(0, &original);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(0, TCSANOW, &term);
	input = readline("minishell> ");
	tcsetattr(0, TCSANOW, &original);
	if (!input || ft_strcmp(input, "") == 0)
	{
		if (!input)
			exit (0);
		free(input);
		return (NULL);
	}
	return (input);
}

int	add_free(char *input)
{
	add_history(input);
	free(input);
	return (1);
}

int	input_checker(char *input)
{
	if (!input)
		return (1);
	if (is_space(input) && add_free(input))
		return (1);
	if (check_line(input))
		return (1);
	return (0);
}

int	order_checker(int *arr, char *input, char **cmd_array)
{
	if (op_order(arr))
	{
		exit_s = 258;
		free(arr);
		free(input);
		free_2d(cmd_array);
		return (1);
	}
	return (0);
}

char	*input_get(char *input)
{
	input = fill_line(input);
	input = add_spaces(input);
	return (input);
}

int	command_valid(t_vars var, char *input)
{
	if (valid_command(var.new))
	{
		free(var.arr);
		free_2d(var.new);
		free(input);
		return (1);
	}
	return (0);
}

void	my_allfree(char *input, t_vars var)
{
	free_2d(var.new);
	free(var.arr);
	free(input);
}

void	close_free(t_vars var, char *input)
{
	close_files(var.commands);
	my_free(var.commands);
	free_all(input, var.new);
	free(var.arr);
}

int	executer(t_vars var, char *input, t_list **env)
{
	if (!var.commands)
	{
		my_allfree(input, var);
		return (1);
	}
	while (var.tmp)
	{
		if (is_heredoc(var.tmp))
			here_docc(var.tmp, *env);
		var.tmp = var.tmp->next;
	}
	if (ft_lstsize(var.commands) > 1)
	{
		multiple_pipes(var.commands, env);
		close_free(var, input);
		return (1);
	}
	if (execute_builtins(var.commands, env))
	{
		close_free(var, input);
		return (1);
	}
	return (0);
}

void	close_free_2(t_vars var, char *input)
{
	close_files(var.commands);
	my_free(var.commands);
	// free_2d(var.new);
	free(var.arr);
	free(input);
}

void	get_input(char *input, t_list **env)
{
	t_vars	var;

	var.delimiter = 0;
	input = signal_handler(input);
	if (input_checker(input))
		return ;
	input = input_get(input);
	var.cmd_array = ft_split(input, ' ');
	var.arr = array_tokens(var.cmd_array, num_elemnts(var.cmd_array));
	if (order_checker(var.arr, input, var.cmd_array))
		return ;
	var.new = expander(var.cmd_array, *env, var.arr);
	free(var.arr);
	var.arr = array_tokens(var.new, num_elemnts(var.new));
	var.new = quote_delete(var.new, &(var.delimiter), var.arr);
	if (command_valid(var, input))
		return ;
	var.commands = list_cmds(var.new, var.arr, &(var.delimiter));
	var.tmp = var.commands;
	add_history(input);
	if (executer(var, input, env))
		return ;
	execute_commands(var.commands, env, var.new);
	close_free_2(var, input);
}

void	shlvl_edit(t_list **env, int op)
{
	char *shlvl;
	t_list *tmp;
	int num;

	tmp = *env;
	while (tmp && ft_strncmpp(tmp->content, "SHLVL=", 5))
		tmp = tmp->next;
	shlvl = ft_strdup(tmp->content + 6);
	num = ft_atoi(shlvl);
	if (op == 0)
		num++;
	else
		num--;
	free(shlvl);
	shlvl = ft_itoa(num);
	// free(tmp->content);
	if (op == 0)
	{
		tmp->content = ft_strjoinn("SHLVL=", shlvl);
		free(shlvl);
		return ;
	}
	else
	{
		tmp->content = ft_strjoinn("SHLVL=", shlvl);
		free(shlvl);
		return ;
	}

}

int main (int ac, char **av, char **envp)
{
	
	char    input;
	t_list *env;

	if (ac > 1)
	{
		write(2, "minishell : ", 12);;
		write(2, av[1], ft_strlenn(av[1]));
		write(2, " : No such file or directory\n", 29);
		exit(127);
	}
	env = make_env(envp);
	shlvl_edit(&env, 0);
	while (1)
	{
		get_input(&input, &env);
	}
}