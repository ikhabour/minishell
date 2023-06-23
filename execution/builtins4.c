/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:11:19 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:11:49 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	unset_valid(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (0);
	while (is_alpha(str[i]) || str[i] == '_')
		i++;
	if (!str[i])
		return (1);
	return (0);
}

int	valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (0);
	if (is_space(str))
		return (0);
	while (str[i])
	{
		if (str[i] == '+' || str[i] == '=')
			break ;
		i++;
	}
	if (i == 0)
		return (0);
	while (i >= 0)
	{
		if (str[i] && !is_alpha(str[i]) && str[i] != '_' && str[i] != '='
			&& str[i] != '+')
			return (0);
		i--;
	}
	return (1);
}

void	identifier_error(char *option)
{
	write(2, "Minishell : unset: `", 20);
	write(2, option, ft_strlenn(option));
	write(2, "': not a valid identifier\n", 26);
	g_sigs.exit_s = 1;
}

void	export_helper(t_bvars var, t_cmds *ptr, t_list *tmp, t_list **env)
{
	var.i = 0;
	while (ptr->option[var.j][var.i])
		var.i++;
	tmp = *env;
	while (tmp && ft_strncmpp(tmp->content, ptr->option[var.j], var.i))
		tmp = tmp->next;
	if (!tmp)
		ft_lstadd_backk(env, ft_lstneww(ft_strdup(ptr->option[var.j])));
}

void	export_helper2(t_bvars var, t_list *tmp, t_cmds *ptr, t_list **env)
{
	char	*temp;

	var.i = 0;
	tmp = *env;
	while (ptr->option[var.j][var.i] != '=')
		var.i++;
	while (tmp && ft_strncmpp(tmp->content, ptr->option[var.j], var.i - 1))
		tmp = tmp->next;
	if (!tmp)
		ft_lstadd_backk(env, ft_lstneww(remove_plus(ptr->option[var.j])));
	else
	{
		temp = tmp->content;
		if (!change_value(tmp->content))
		{
			tmp->content = ft_strjoinn(temp, ptr->option[var.j] + (var.i));
			free(temp);
		}
		else
		{
			tmp->content = ft_strjoinn(temp, ptr->option[var.j] + (var.i + 1));
			free(temp);
		}
	}
}
