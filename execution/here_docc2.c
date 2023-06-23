/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docc2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/23 02:19:14 by ikhabour          #+#    #+#             */
/*   Updated: 2023/06/23 02:19:24 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	start_index_ex(char *cmd, int *i)
{
	if ((*i > 0) && cmd[*i] == '$')
		*i -= 1;
	return (*i);
}

static char	*ft_expand_ex(char *cmd, t_list *env, int *i)
{
	int			limiter;
	static char	*full_str;
	char		*lineup;
	char		*str;
	char		*value;

	limiter = limiter_value_ex(i, cmd);
	str = ft_substr(cmd, (*i + 1), limiter - (*i + 1));
	value = env_value(str, env);
	if (value)
	{
		lineup = ft_substr(cmd, 0, (*i));
		full_str = ft_strjoin(lineup, value);
	}
	else
	{
		lineup = ft_substr(cmd, 0, (*i));
		full_str = ft_strdup(lineup);
	}
	free(lineup);
	cmd = cmd_expand_ex(cmd, limiter, lineup, full_str);
	*i = ft_strlen(full_str);
	(free(full_str), free(str));
	*i = start_index_ex(cmd, i);
	return (cmd);
}

static char	*ft_expand_exit_ex(char *cmd, t_list *env, int *i)
{
	char	*str;
	char	*lineup;
	char	*value;
	int		limiter;
	char	*full_str;

	(void)env;
	limiter = *(i) + 2;
	str = ft_substr(cmd, limiter, 1000);
	value = ft_itoa(g_sigs.exit_s);
	lineup = ft_substr(cmd, 0, *i);
	full_str = ft_strjoin(lineup, value);
	free(lineup);
	free(cmd);
	cmd = ft_strjoin(full_str, str);
	free(full_str);
	free(value);
	free(str);
	if ((*i > 0) && cmd[*i] == '$')
		*i -= 1;
	return (cmd);
}

char	*expand_heredoc(char *input, t_list *env)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] && input[i + 1] && input[i] == '$' && input[i + 1] == '$')
			i++;
		if (input[i] && input[i + 1] && input[i] == '$' && input[i + 1] == '?')
			input = ft_expand_exit_ex(input, env, (&i));
		if (input[i] && input[i + 1] && input[i] == '$' && input[i + 1] != '?')
			input = ft_expand_ex(input, env, (&i));
		if (input[i] && input[i + 1] && input[i] == '$' && input[i + 1] == '?')
			input = ft_expand_exit_ex(input, env, (&i));
		if (input[i])
			i++;
	}
	return (input);
}

void	display_prompt(t_list *files, int fd, t_list *env)
{
	char		*input;
	t_filetype	*ptr;
	char		*joined;

	ptr = (t_filetype *)files->content;
	while (1)
	{
		input = readline("> ");
		if (!input || !ft_strcmp(input, ptr->file_name))
			break ;
		if (ptr->has_quotes == 1)
			joined = ft_strjoinn(input, "\n");
		else
			joined = ft_strjoinn(expand_heredoc(input, env), "\n");
		ft_putstr_fd(joined, fd);
		free(joined);
	}
	close(fd);
}
