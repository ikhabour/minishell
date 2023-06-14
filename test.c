#include "minishell.h"

int	ft_strlenn(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}


int	main()
{
	printf("len : %d\n", ft_strlenn("HELLO"));
}