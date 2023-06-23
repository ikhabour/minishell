/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:12:25 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:12:35 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_helper3(t_bvars var, t_list **tmp, t_list **env, t_cmds *ptr)
{
	var.i = 0;
	*tmp = *env;
	while (ptr->option[var.j][var.i] != '=')
		var.i++;
	while (*tmp && ft_strncmpp((*tmp)->content, ptr->option[var.j], var.i))
		*tmp = (*tmp)->next;
}

int	export_and_files(t_cmds *ptr, t_list **env, int fd)
{
	if (ptr->files)
		(open_files(ptr), dup_fds(ptr));
	g_sigs.exit_s = 0;
	if (!ptr->option[0])
	{
		(print_export(env), dup2(fd, 1));
		close(fd);
		return (1);
	}
	return (0);
}

void	export_while(t_cmds *ptr, t_bvars var, t_list *tmp, t_list **env)
{
	while (ptr->option[var.j])
	{
		if (!valid_identifier(ptr->option[var.j]))
		{
			identifier_error(ptr->option[var.j]);
			var.j++;
			continue ;
		}
		if (append_value(ptr->option[var.j]))
			export_helper2(var, tmp, ptr, env);
		else if (change_value(ptr->option[var.j]))
		{
			export_helper3(var, &tmp, env, ptr);
			if (!tmp)
			{
				ft_lstadd_backk(env, ft_lstneww(ptr->option[var.j]));
				continue ;
			}
			tmp->content = ft_strdup(ptr->option[var.j]);
		}
		else
			export_helper(var, ptr, tmp, env);
		var.j++;
	}
}

void	execute_export(t_list *cmd, t_list **env)
{
	t_cmds	*ptr;
	t_bvars	var;
	t_list	*tmp;
	int		fd;

	ptr = (t_cmds *)cmd->content;
	init_bvars(&var);
	tmp = *env;
	fd = dup(1);
	if (export_and_files(ptr, env, fd))
		return ;
	export_while(ptr, var, tmp, env);
	(dup2(fd, 1), close(fd));
}

int	unset_helper(t_list **curr, t_cmds *ptr, t_list **env, int i)
{
	int	fd;

	fd = dup(1);
	*curr = *env;
	if (*curr && !ft_strncmpp((*curr)->content, ptr->option[i],
			ft_strlenn(ptr->option[i])))
	{
		*env = (*curr)->next;
		(free(curr), dup2(fd, 1));
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}
