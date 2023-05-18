/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:42:06 by ikhabour          #+#    #+#             */
/*   Updated: 2022/12/20 18:51:57 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define CMD_NOT_FOUND "command not found : "
# define FILE_ERROR "Error opening file : "
# define ARGUMENTS_ERROR "Please enter 4 arguments!\n"

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/errno.h>
# include <unistd.h>

size_t	ft_strlen(const char *str);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strjoin(const char *s1, const char *s2);
void	ft_wait(int pid1, int *fd, int pid);
char	*bring_path(char **cmd, char **envp);
char	**ft_split(const char *s, char c);

#endif