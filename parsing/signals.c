/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 23:06:44 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/16 18:50:54 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler(int sig)
{
	(void) sig;
	write (1, "\n", 2);
	write (1, "Minishell> ", 11);
	// write (1, "new prompt\n", 12);
	// fflush(stdout);
}

void	sig_handler()
{
	// struct sigaction	sa;
	// sa.sa_handler = handler;
	// sa.sa_flags = SA_RESTART;
	// sigaction(SIGINT, &sa, NULL);
	
	signal(SIGINT, handler);
}