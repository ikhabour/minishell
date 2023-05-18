/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:59:11 by ikhabour          #+#    #+#             */
/*   Updated: 2022/12/21 23:36:42 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*bring_path(char **cmd, char **envp)
{
	char	**paths;
	int		i;
	char	*save;
	char	*cmd1;
	char	*f_path;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			save = envp[i] + 5;
		i++;
	}
	i = 0;
	paths = ft_split(save, ':');
	cmd1 = ft_strjoin("/", cmd[0]);
	while (paths[i])
	{
		f_path = ft_strjoin(paths[i], cmd1);
		if (access(f_path, X_OK) == 0)
			break ;
		free(f_path);
		i++;
	}
	return (f_path);
}

void	ft_wait(int pid, int *fd, int pid1)
{
	int	status;

	close(fd[0]);
	close(fd[1]);
	waitpid(pid, NULL, 0);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
}

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!haystack && !len)
		return (0);
	else if (*needle == '\0')
		return ((char *)haystack);
	while (haystack[i] && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && i + j < len)
		{
			if (needle[j + 1] == '\0')
				return ((char *)haystack + i);
			j++;
		}
		i++;
	}
	return (0);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	char	*new;
	int		j;

	if (!s1 || !s2)
		return (0);
	i = 0;
	j = ft_strlen(s1);
	new = malloc(ft_strlen(s1) + ft_strlen(s2) * sizeof(char) + 1);
	if (new == NULL)
		return (NULL);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2[i])
		new[j++] = s2[i++];
	new[j] = '\0';
	return (new);
}
