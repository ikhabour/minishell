/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:36:26 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 02:51:38 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_sig	g_sigs;

void	close_free_2(t_vars var, char *input)
{
	close_files(var.commands);
	my_free(var.commands);
	free(var.arr);
	free(input);
}

void	signal_exit(void)
{
	if (g_sigs.execc == 0)
		g_sigs.exit_s = 1;
}

void	get_input(char *input, t_list **env)
{
	t_vars	var;

	var.delimiter = 0;
	input = signal_handler(input, var.commands);
	add_history(input);
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
	g_sigs.execc = 1;
	if (!executer(var, input, env))
		(execute_commands(var.commands, env, var.new), close_free_2(var,
				input));
	signal_exit();
}

void	shlvl_edit(t_list **env, int op)
{
	char	*shlvl;
	t_list	*tmp;
	int		num;

	tmp = *env;
	while (tmp && ft_strncmpp(tmp->content, "SHLVL=", 5))
		tmp = tmp->next;
	shlvl = ft_strdup(tmp->content + 6);
	num = ft_atoi(shlvl);
	(op == 0) && num++;
	(op != 0) && num--;
	free(shlvl);
	shlvl = ft_itoa(num);
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

int	main(int ac, char **av, char **envp)
{
	char	input;
	t_list	*env;

	if (ac > 1)
	{
		write(2, "minishell : ", 12);
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
