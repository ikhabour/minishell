/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 23:06:44 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 02:04:49 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler(int sig)
{
	(void)sig;
	g_sigs.exit_s = 130;
	write(1, "\n", 1);
	if (g_sigs.readline == 1)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else
		g_sigs.execc = 0;
}

void	sig_quit(int sig)
{
	(void)sig;
	if (g_sigs.process == 1)
	{
		if (g_sigs.readline == 0)
			g_sigs.execc = 1;
		write(1, "Quit\n", 5);
	}
	rl_redisplay();
}

void	sig_handler(void)
{
	signal(SIGINT, handler);
}
