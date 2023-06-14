/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 15:02:45 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/15 00:19:29 by ikhabour         ###   ########.fr       */
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

char	**add_to_array(char **arr, char *value)
{
	int i;
	char **new_arr;

	i = 0;
	if (!arr)
	{
		arr = malloc(sizeof(char *) * 2);
		arr[0] = ft_strdupp(value);
		arr[1] = NULL;
		return (arr);
	}
	else
	{
		while (arr && arr[i])
			i++;
		new_arr = malloc(sizeof(char *) * (i + 1));
		new_arr[i] = NULL;
		i = 0;
		while (arr[i])
		{
			new_arr[i] = ft_strdupp(arr[i]);
			i++;
		}
		new_arr[i] = ft_strdup(value);
		free_2d(arr);
	}
	return (new_arr);
}

char	*expand_value(char *str, t_list *env)
{
	int i;
	char *old_val;
	char	*new_val;
	char *join;

	i = 0;
	while (str[i] != '$')
		i++;
	old_val = malloc(sizeof(char) * i + 1);
	i = 0;
	while (str[i] != '$')
	{
		old_val[i] = str[i];
		i++;
	}
	old_val[i] = '\0';
	new_val = env_value(str + i + 1, env);
	join = new_val;
	new_val = ft_strjoinn(old_val, new_val);
	// free(join);
	return(new_val);
}

char	*write_values(char **arr)
{
	int len;
	int i;
	char *str;
	int j;
	int x;

	len = 0;
	x = 0;
	i = 0;
	while (arr[i])
	{
		len += ft_strlenn(arr[i]) + 1;
		i++;
	}
	str = malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (arr[i])
	{
		j = 0;
		while (arr[i][j])
			str[x++] = arr[i][j++];
		if (arr[i])
			str[x++] = ' ';
		i++;
	}
	str[x] = '\0';
	return (str);

}

char	*here_doc_expand(char *input, t_list **env)
{
	char	**str;
	int i;
	int j;
	char	*value;
	char **new_arr = NULL;
	int exp;

	i = 0;
	exp = 0;
	str = ft_splitt(input, ' ');
	// free(input);
	i = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (str[i][j] == '$')
			{
				exp = 1;
				break ;
			}
			j++;
		}
		if (exp == 1)
		{
			value = expand_value(str[i], *env);
			new_arr = add_to_array(new_arr, value);
			exp = 0;
		}
		else
			new_arr = add_to_array(new_arr, str[i]);
		i++;
	}
	input = write_values(new_arr);
	return (input);
}


void	display_prompt(t_list *files, int fd, t_list **env)
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
		{
			joined = here_doc_expand(input, env);
			joined = ft_strjoinn(joined, "\n");
		}
		ft_putstr_fd(joined, fd);
		free(joined);
		free(input);

	}
	close(fd);
}

void	here_docc(t_list *commands, t_list **env)
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
			display_prompt(tmp, fds[i][1], env);
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