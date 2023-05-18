/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:41:55 by ikhabour          #+#    #+#             */
/*   Updated: 2022/12/21 23:32:12 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fd_redirect(int file, int *fd, int op)
{
	if (op == 0)
	{
		dup2(file, 0);
		dup2(fd[1], 1);
		close(fd[0]);
	}
	if (op == 1)
	{
		dup2(file, 1);
		dup2(fd[0], 0);
		close(fd[1]);
	}
}

int	open_file(char *filename, int op)
{
	int	fd;

	fd = -1;
	if (op == 0)
		fd = open(filename, O_RDWR);
	if (op == 1)
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		write(2, FILE_ERROR, ft_strlen(FILE_ERROR));
		write(2, filename, ft_strlen(filename));
		write(2, "\n", 1);
		exit(9);
	}
	return (fd);
}

void	exec_cmd1(char **argv, int *fd, char **envp)
{
	int		file;
	char	**cmd;
	int		exec;

	file = open_file(argv[1], 0);
	cmd = ft_split(argv[2], ' ');
	fd_redirect(file, fd, 0);
	if (access(cmd[0], X_OK) == 0)
		execve(cmd[0], cmd, envp);
	exec = execve(bring_path(cmd, envp), cmd, envp);
	if (exec == -1)
	{
		write(2, CMD_NOT_FOUND, ft_strlen(CMD_NOT_FOUND));
		write(2, argv[2], ft_strlen(argv[2]));
		write(2, "\n", 1);
		exit(127);
	}
}

void	exec_cmd2(char **argv, int *fd, char **envp)
{
	int		file;
	char	**cmd;
	int		exec;

	file = open_file(argv[4], 1);
	cmd = ft_split(argv[3], ' ');
	fd_redirect(file, fd, 1);
	if (access(cmd[0], X_OK) == 0)
		execve(cmd[0], cmd, envp);
	exec = execve(bring_path(cmd, envp), cmd, envp);
	if (exec == -1)
	{
		write(2, CMD_NOT_FOUND, ft_strlen(CMD_NOT_FOUND));
		write(2, argv[3], ft_strlen(argv[3]));
		write(2, "\n", 1);
		exit(127);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];
	int	pid[2];

	if (argc != 5)
	{
		write(2, ARGUMENTS_ERROR, ft_strlen(ARGUMENTS_ERROR));
		exit(22);
	}
	if (pipe(fd) == -1)
		perror("Pipe failed!");
	pid[0] = fork();
	if (pid[0] == -1)
		perror("Fork failed!");
	if (pid[0] == 0)
		exec_cmd1(argv, fd, envp);
	else
	{
		pid[1] = fork();
		if (pid[1] == -1)
			perror("Fork failed!");
		if (pid[1] == 0)
			exec_cmd2(argv, fd, envp);
		else
			ft_wait(pid[0], fd, pid[1]);
	}
}
