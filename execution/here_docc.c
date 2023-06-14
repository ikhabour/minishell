/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 15:02:45 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/14 15:54:04 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	close_files(t_list *commands)
{
	t_list *tmp;
	t_list *tmp1;
	t_filetype *files;
	t_cmds *ptr;

	tmp1 = commands;

	ptr = (t_cmds *)tmp1->content;
	while (tmp1)
	{
		if (!ptr->files)
			return ;
		tmp = ptr->files;
		files = (t_filetype *)tmp->content;
		while (tmp)
		{
			if (files->fd != -1 && files->fd != 0)
				close(files->fd);
			tmp = tmp->next;
			if (tmp)
				files = (t_filetype *)tmp->content;
		}
		tmp1 = tmp1->next;
		if (tmp1)
			ptr = (t_cmds *)tmp1->content;
	}
}

int	is_heredoc(t_list *commands)
{
	t_cmds *ptr;
	t_filetype *files;
	t_list *tmp;

	ptr = (t_cmds *)commands->content;
	if (!ptr->files)
		return (0);
	tmp = ptr->files;
	files = (t_filetype *)tmp->content;
	while (tmp)
	{
		if (!ft_strcmp(files->type, "DELIMITER"))
			return (1);
		tmp = tmp->next;
		if (tmp)
			files = (t_filetype *)tmp->content;
	}
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}


void	display_prompt(t_list *files, int fd)
{
	char *input;
	t_filetype *ptr;
	ptr = (t_filetype *)files->content;
	char *joined;

	while (1)
	{
		input = readline("> ");
		if (!input || !ft_strcmp(input, ptr->file_name))
			break ;
		joined = ft_strjoinn(input, "\n");
		ft_putstr_fd(joined, fd);
		free(joined);
		free(input);

	}
	close(fd);
}

void	here_docc(t_list *commands)
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
	if (!tmp)
		return ;
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
		fds[i++] = malloc(sizeof(int) * 2);
	fds[i] = 0;
	i = 0;
	while (i < docs)
	{
		if (pipe(fds[i]) < 0)
			return ;
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
			p->fd = -2;
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
	free_int_arr(fds, docs);
}