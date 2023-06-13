/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:36:26 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/13 17:54:29 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_s = 0;
int return_val = 0;

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
	// printf("this is the length : %d\n", len);
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
	free(input);
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
	int	flag = 0;

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
		else if ((i > 0) && (ft_strcmp(cmd_array[i - 1], "<>") == 0))
			cmd_token[i] = R_OUT_FILE;
		else if ((i > 0) && (ft_strcmp(cmd_array[i - 1], "<") == 0))
			cmd_token[i] = R_IN_FILE;
		else
		{
			cmd_token[i] = CMD_NAME;
			if ((i > 0) && (cmd_token[i - 1] == CMD_NAME || cmd_token[i - 1] == CMD_ARG) && 
				ft_strcmp(cmd_array[i], ">") && ft_strcmp(cmd_array[i], "<") && 
				ft_strcmp(cmd_array[i], ">>") && ft_strcmp(cmd_array[i], "<<") && 
				ft_strcmp(cmd_array[i], "|"))
				cmd_token[i] = CMD_ARG;
			else if ((i > 0) && flag == 1 && (cmd_token[i - 1] == R_OUT_FILE || cmd_token[i - 1] == R_IN_FILE || cmd_token[i - 1] == R_APP_FILE ||
					cmd_token[i - 1] == CMD_ARG) && ft_strcmp(cmd_array[i], ">") && ft_strcmp(cmd_array[i], "<") && 
					ft_strcmp(cmd_array[i], ">>") && ft_strcmp(cmd_array[i], "<<") && 
					ft_strcmp(cmd_array[i], "|"))
				cmd_token[i] = CMD_ARG;
				flag = 1;
		}
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

void	get_input(char *input, t_list **env)
{
	int		len;
	char	**cmd_array;
	char	**new;
	t_list	*commands;
	t_list *tmp;
	struct termios	term;
	struct termios	original;
	
	// t_cmds *ptr;
	int		*arr;
	(void) env;


	sig_handler();
	tcgetattr(0, &term);
	tcgetattr(0, &original);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(0, TCSANOW, &term);
	input = readline("minishell> ");
	tcsetattr(0, TCSANOW, &original);

	// term.c_lflag &= ~(ECHOCTL);
	// tcsetattr(0, TCSANOW, &term);
	// rl_catch_sigint = 0;
	// input = readline("Minishell> ");
	if (!input || ft_strcmp(input, "") == 0)
	{
		if (!input)
			exit(0);
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
	if (check_line(input))
	{
		free (input);
		return ;
	}
	input = fill_line(input, len);
	input = add_spaces(input);
	// printf("this is the line : %s\n", input);
	cmd_array = ft_split(input, ' ');
	new = expander(cmd_array, *env);
	split_print(new);
	// // split_print(cmd_array);
	arr = array_tokens(new, num_elemnts(new));
	// array_printer(arr);
	if (op_order(arr))
	{
		exit_s = 258;
		free(input);
		free_2d(new);
		return ;
	}
	new = quote_delete(new);
	commands = list_cmds(new, arr);
	tmp = commands;
	print_list(commands);
	add_history(input);
	if (!commands)
	{
		free_2d(new);
		free(arr);
		free(input);
		return ;
	}
	while (tmp)
	{
		if (is_heredoc(tmp))
			here_docc(tmp);
		tmp = tmp->next;
	}
	if (ft_lstsize(commands) > 1)
	{
		multiple_pipes(commands, env);
		close_files(commands);
		free_all(input, new);
		free(arr);
		my_free(commands);
		return ;
	}
	if (execute_builtins(commands, env))
	{
		my_free(commands);
		close_files(commands);
		free_all(input, new);
		free(arr);
		if (return_val != 0)
		{
			exit_s = return_val;
			return ;
		}
		exit_s = 0;
		return ;
	}
	execute_commands(commands, env, new);
	close_files(commands);
	my_free(commands);
	free(arr);
	free_2d(new);
	free(input);
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
	struct termios term;

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
		tcgetattr(0, &term);
		term.c_lflag &= ~(ECHOCTL);
		tcsetattr(0, TCSANOW, &term);
		// signal(SIGINT, handler);
		get_input(&input, &env);
	}
}