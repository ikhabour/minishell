/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 15:02:45 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/11 19:10:29 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}


void	display_prompt(t_list *files, int fd)
{
	char *input;
	t_filetype *ptr;
	ptr = (t_filetype *)files->content;

	while (1)
	{
		input	= readline("> ");
		if (!input || !ft_strcmp(input, ptr->file_name))
			break ;
		ft_putstr_fd(input, fd);

	}
	close(fd);
}

int	here_docc(t_list *commands)
{
	int pid;
	int **fds;
	int docs;
	int i;
	t_list *tmp;
	t_cmds *ptr;
	t_filetype *p;
	t_filetype *last_heredoc;

	ptr = (t_cmds *)commands->content;
	docs = 0;
	tmp = ptr->files;
	p = (t_filetype *)tmp->content;
	i = 0;
	while (tmp)
	{
		if (!ft_strcmp(p->type, "DELIMITER"))
		{
			last_heredoc = p;
			docs++;
		}
		tmp = tmp->next;
		if (tmp)
			p = (t_filetype *)tmp->content;
	}
	fds = malloc(sizeof(int *) * (docs + 1));
	while (i < docs)
	{
		fds[i] = malloc(sizeof(int) * 2);
		i++;
	}
	fds[i] = 0;
	i = 0;
	while (i < docs)
	{
		if (pipe(fds[i]) < 0)
			return (0);
		i++;
	}
	tmp = ptr->files;
	pid = fork();
	if (pid == -1)
		msg_exit("Fork", "Failed!\n", 1);
	i = 0;
	if (pid == 0)
	{
		while (tmp && i < docs)
		{
			display_prompt(tmp, fds[i][1]);
			close(fds[i][0]);
			tmp = tmp->next;
			i++;
		}
		exit(0);
	}
	waitpid(pid, NULL, 0);

	while (tmp)
	{
		if (!ft_strcmp(p->type, "DELIMITER"))
			p->fd = -1;
		tmp = tmp->next;
		if (tmp)
			p = (t_filetype *)tmp->content;
	}
	while (i < docs)
	{
		close(fds[i][1]);
		if (i == docs - 1)
			break ;
		close(fds[i][0]);
		i++;
	}
	last_heredoc->fd = fds[i][0];
	return (1);
}