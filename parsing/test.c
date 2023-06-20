/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 21:23:05 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/20 05:17:25 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(void)
{
	t_list	*node;
	t_list	*list;

	list = NULL;
	for (int i = 0; i < 5; i++)
		my_lstadd_back(&list, my_lstnew(ft_strdup("hello")));

	node = list;
	while (node)
	{
		printf("%s\n", node->content);
		node = node->next;
	}
}
