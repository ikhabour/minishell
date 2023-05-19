/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_3_linked_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 01:38:21 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/17 16:42:31 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*ft_lstnew(char *name, char *value, t_env *new)
{
	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->env_name = name;
	new->env_value = value;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*lastnode;

	if (!lst)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		lastnode = *lst;
		while (lastnode->next != NULL)
				lastnode = lastnode->next;
			lastnode->next = new;
	}
}

t_list	*my_lstnew(void *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

void	my_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*lastnode;

	if (!lst)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		lastnode = *lst;
		while (lastnode->next != NULL)
				lastnode = lastnode->next;
			lastnode->next = new;
	}
}
