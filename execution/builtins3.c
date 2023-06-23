/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:10:42 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:10:57 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_string(char *str)
{
	int	i;

	i = 0;
	write(1, "declare -x ", 11);
	if (change_value(str))
	{
		while (str[i] && str[i] != '=')
		{
			write(1, &str[i], 1);
			i++;
		}
		write(1, &str[i], 1);
		write(1, "\"", 1);
		i++;
		while (str[i])
		{
			write(1, &str[i], 1);
			i++;
		}
		write(1, "\"", 1);
	}
	else
		printf("%s", str);
}

void	print_env(t_list **env)
{
	t_list	*tmp;

	tmp = *env;
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
}

void	print_export(t_list **env)
{
	t_list	*tmp;

	tmp = *env;
	while (tmp)
	{
		print_string(tmp->content);
		printf("\n");
		tmp = tmp->next;
	}
}

int	var_exists(char *var, t_list **env)
{
	t_list	*tmp;

	tmp = *env;
	while (ft_strncmp(tmp->content, var, ft_strlenn(var)))
		tmp = tmp->next;
	if (!tmp)
		return (0);
	return (1);
}

int	is_alpha(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
