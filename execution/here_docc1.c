/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docc1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:18:22 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:18:53 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	doc_child_process(t_filetype *p, t_heredoc var, t_list *env)
{
	signal(SIGINT, SIG_DFL);
	p = (t_filetype *)var.tmp->content;
	while (g_sigs.execc && var.tmp && var.i < var.docs)
	{
		if (!ft_strcmp(p->type, "DELIMITER"))
		{
			display_prompt(var.tmp, var.fds[var.i][1], env);
			close(var.fds[var.i][0]);
			var.i++;
		}
		var.tmp = var.tmp->next;
		if (var.tmp)
			p = (t_filetype *)var.tmp->content;
	}
	exit(0);
}

void	here_doc_helper(t_heredoc var, t_filetype *p, t_filetype **last_heredoc)
{
	waitpid(var.pid, NULL, 0);
	while (var.tmp)
	{
		if (!ft_strcmp(p->type, "DELIMITER"))
			p->fd = -2;
		var.tmp = var.tmp->next;
		if (var.tmp)
			p = (t_filetype *)var.tmp->content;
	}
	while (var.i < var.docs)
	{
		close(var.fds[var.i][1]);
		if (var.i == var.docs - 1)
			break ;
		close(var.fds[var.i][0]);
		var.i++;
	}
	(*last_heredoc)->fd = var.fds[var.i][0];
	free_int_arr(var.fds, var.docs);
}

void	count_docs(t_heredoc *var, t_filetype **last_heredoc, t_filetype **p)
{
	while (var->tmp)
	{
		if (!ft_strcmp((*p)->type, "DELIMITER"))
		{
			*last_heredoc = *p;
			var->docs++;
		}
		var->tmp = var->tmp->next;
		if (var->tmp)
			*p = (t_filetype *)var->tmp->content;
	}
}

void	here_doc_helper1(t_heredoc *var, t_cmds *ptr)
{
	var->fds[var->i] = 0;
	var->i = 0;
	while (var->i < var->docs)
	{
		if (pipe(var->fds[var->i]) < 0)
			return ;
		var->i++;
	}
	var->tmp = ptr->files;
}

void	here_docc(t_list *commands, t_list *env)
{
	t_heredoc	var;
	t_cmds		*ptr;
	t_filetype	*p;
	t_filetype	*last_heredoc;

	ptr = (t_cmds *)commands->content;
	var.docs = 0;
	var.tmp = ptr->files;
	if (!var.tmp)
		return ;
	p = (t_filetype *)var.tmp->content;
	last_heredoc = p;
	var.i = 0;
	count_docs(&var, &last_heredoc, &p);
	var.fds = malloc(sizeof(int *) * (var.docs + 1));
	while (var.i < var.docs)
		var.fds[var.i++] = malloc(sizeof(int) * 2);
	here_doc_helper1(&var, ptr);
	var.pid = fork();
	if (var.pid == -1)
		msg_exit("Fork", "Failed!\n", 1);
	var.i = 0;
	if (var.pid == 0)
		doc_child_process(p, var, env);
	here_doc_helper(var, p, &last_heredoc);
}
