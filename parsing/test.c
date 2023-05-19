#include "minishell.h"

int main()
{
	t_list	*node, *list;

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