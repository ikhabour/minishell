/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:12:55 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:13:09 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset_helper1(t_list **curr, t_list **prev, int fd)
{
	if (*curr == NULL)
	{
		(dup2(fd, 1), close(fd));
		return (1);
	}
	(*prev)->next = (*curr)->next;
	free(*curr);
	return (0);
}

void	sub_while(t_list **curr, t_cmds *ptr, int i, t_list **prev)
{
	while (*curr && ft_strncmpp((*curr)->content, ptr->option[i],
			ft_strlenn(ptr->option[i])))
	{
		*prev = *curr;
		*curr = (*curr)->next;
	}
}

int	unset_while(t_cmds *ptr, t_list *curr, t_list *prev, t_list **env)
{
	int	i;
	int	fd;

	i = 0;
	fd = dup(1);
	while (ptr->option[i])
	{
		if (!unset_valid(ptr->option[i]))
		{
			identifier_error(ptr->option[i]);
			i++;
			continue ;
		}
		prev = NULL;
		if (unset_helper(&curr, ptr, env, i))
			return (1);
		sub_while(&curr, ptr, i, &prev);
		if (unset_helper1(&curr, &prev, fd))
			return (1);
		i++;
	}
	return (close(fd), 0);
}

void	execute_unset(t_list *cmd, t_list **env)
{
	t_list	*curr;
	t_list	*prev;
	t_cmds	*ptr;
	int		fd;

	ptr = (t_cmds *)cmd->content;
	fd = dup(1);
	if (ptr->files)
		(open_files(ptr), dup_fds(ptr));
	g_sigs.exit_s = 0;
	prev = NULL;
	curr = NULL;
	if (!ptr->option)
	{
		(dup2(fd, 1), close(fd));
		return ;
	}
	if (unset_while(ptr, curr, prev, env))
		return ;
	(dup2(fd, 1), close(fd));
}

void	write_exit(void)
{
	if (g_sigs.process == 0)
		write(2, "exit\n", 6);
}
