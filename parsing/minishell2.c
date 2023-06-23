/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:50:45 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:52:05 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*signal_handler(char *input, t_list *commands)
{
	struct termios	term;
	struct termios	original;

	(void)commands;
	sig_handler();
	signal(SIGQUIT, sig_quit);
	tcgetattr(0, &term);
	tcgetattr(0, &original);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(0, TCSANOW, &term);
	g_sigs.readline = 1;
	input = readline("minishell> ");
	g_sigs.readline = 0;
	if (!input || ft_strcmp(input, "") == 0)
	{
		if (!input)
			(write(2, "exit\n", 6), exit(0));
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
		g_sigs.exit_s = 258;
		free(arr);
		free(input);
		free_2d(cmd_array);
		return (1);
	}
	return (0);
}
