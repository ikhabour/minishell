/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:36:24 by bhazzout          #+#    #+#             */
/*   Updated: 2023/05/18 15:44:52 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/stat.h>

typedef struct t_env
{
	char			*env_name;
	char			*env_value;
	struct t_env	*next;
} 					t_env;

typedef struct s_filetype
{
	int				fd;
	char			*red;
    char            *type;
    char            *file_name;
}                    t_filetype;

typedef struct s_cmds
{
    char            *cmd_name;
    char            **option;
    t_filetype        files;
}                    t_cmds;

typedef struct s_list1
{
   	void			*content;
    struct s_list1    *next;
}                    t_list;

typedef enum t_enum
{
	CMD_NAME = 1,
	CMD_ARG,
	R_IN_SIG,
	R_OUT_SIG,
	R_OUT_FILE,
	PIPE,
	R_APP_SIG,
	R_APP_FILE,
	R_IN_FILE,
	HEREDOC_SIG,
	HEREDOC_LIM,
	EMPTY,
} t_enum;

char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);
int		ft_strlen(const char *str);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	get_env(char **env);
t_env	*ft_lstnew(char *name, char *value, t_env *new);
void	ft_lstadd_back(t_env **lst, t_env *new);
char	*add_spaces(char *input);
char	*ft_substr(char const *s, int start, int len);
char	*ft_strjoin(const char *s1, const char *s2);
int		calc_words(char *s, char c);
int		ft_wordlen(char *s, int i, char c);
char	*ft_charge(char *s, int i, char c, int flag);
void	*freeini(char **string, int len);
char	**ft_split(char *s, char sep);
int		ft_strncmp(const char *s1, const char *s2, int n);
int		ft_strcmp(char *s1, char *s2);
int 	check_pipe(char *input);
int		check_redirec_op(char *input);
int 	check_line(char *input);
char	*skip_spaces(char *input);
int		check_quotes(char *input);
int		check_outside(int count);
void	op_order(int *token);
int		is_outside(int flag, char c);
char	**quote_delete(char **cmd);
void	expander(char **cmd, char **env);
int		ft_envcmp(char *s1, char *s2, int length);
t_list	**list_cmds(char **cmd_array, int *arr);
t_list	*my_lstnew(void *content);
void	my_lstadd_back(t_list **lst, t_list *new);
void	split_print(char **input);

#endif