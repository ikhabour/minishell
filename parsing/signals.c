/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 23:06:44 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/18 01:21:14 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler(int sig)
{
	(void) sig;
	exit_s = 1;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	
}

void	sig_handler()
{
	// struct sigaction	sa;
	// sa.sa_handler = handler;
	// sa.sa_flags = SA_RESTART;
	// sigaction(SIGINT, &sa, NULL);
	
	signal(SIGINT, handler);
}