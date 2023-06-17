/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 15:02:45 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/17 15:44:15 by ikhabour         ###   ########.fr       */
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

static char	*ft_expand(char *cmd, t_list *env, int *i)
{
	int		limiter;
	static char *full_str;
	char	*lineup;
	char	*str;
	char	*value;

	limiter = *i + 1;
	while (cmd[limiter] && (ft_isalnum(cmd[limiter]) || ft_ischar(cmd[limiter])))
	{
		limiter++;
	}
	str = ft_substr(cmd, (*i + 1), limiter - (*i + 1));
	value = env_value(str, env);
	if (value)
	{
		lineup = ft_substr(cmd, 0, (*i));
		full_str = ft_strjoin(lineup, value);
	}
	else
	{
		lineup = ft_substr(cmd, 0, (*i));
		full_str = ft_strdup(lineup);
	}
	free(lineup);
	if (cmd[limiter])
	{
		if (cmd[limiter - 1] == '$')
			lineup = ft_substr(cmd, limiter - 1, 1000);
		else
			lineup = ft_substr(cmd, limiter, 1000);
		free(cmd);
		cmd = ft_strjoin(full_str, lineup);
		free(lineup);
	}
	else
	{
		free(cmd);
		cmd = ft_strdup(full_str);
	}
	*i = ft_strlen(full_str);
	free(full_str);
	free(str);
	if ((*i > 0) && cmd[*i] == '$')
            *i -= 1;
	return(cmd);
}

static char	*ft_expand_exit(char *cmd, t_list *env, int *i)
{
	(void) env;
	char	*str;
	char	*lineup;
	char	*value;
	int		limiter;
	char	*full_str;

	limiter = *(i) + 2;
	str = ft_substr(cmd, limiter, 1000);
	value = ft_itoa(exit_s);
	lineup = ft_substr(cmd, 0, *i);
	full_str = ft_strjoin(lineup, value);
	free(lineup);
	free(cmd);
	cmd = ft_strjoin(full_str, str);
	free(full_str);
	free(value);
	free(str);
	if ((*i > 0) && cmd[*i] == '$')
            *i -= 1;
	return (cmd);
}

char	*expand_heredoc(char *input, t_list *env)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] && input[i + 1] && input[i] == '$' && input[i + 1] == '$')
			i++;
		if (input[i] && input[i + 1] && input[i] == '$' && input[i + 1] == '?')
		{
			input = ft_expand_exit(input, env, (&i));
		}
		if (input[i] && input[i + 1] && input[i] == '$' && input[i + 1] != '?')
		{
			input = ft_expand(input, env, (&i));
		}
		if (input[i] && input[i + 1] && input[i] == '$' && input[i + 1] == '?')
		{
			input = ft_expand_exit(input, env, (&i));
		}
		if (input[i])
			i++;
	}
	return (input);
}

void	display_prompt(t_list *files, int fd, t_list *env)
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
		if (ptr->has_quotes == 1)
			joined = ft_strjoinn(input, "\n");
		else
			joined = ft_strjoinn(expand_heredoc(input, env), "\n");
		ft_putstr_fd(joined, fd);
		free(joined);
		// free(input);
	}
	close(fd);
}

void	here_docc(t_list *commands, t_list *env)
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
		p = (t_filetype *)tmp->content;
		while (tmp && i < docs)
		{
			if (!ft_strcmp(p->type, "DELIMITER"))
			{
				display_prompt(tmp, fds[i][1], env);
				close(fds[i][0]);
				i++;
			}
			tmp = tmp->next;
			if (tmp)
				p = (t_filetype *)tmp->content;
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