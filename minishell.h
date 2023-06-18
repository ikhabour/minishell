/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bhazzout <bhazzout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:36:24 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/18 01:19:04 by bhazzout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// int	EXIT_S = 0;
// #define EXIT_S 0

extern int			exit_s;
extern int			return_val;

# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <unistd.h>
# include <signal.h>
# include <termios.h>
#include <sys/ioctl.h>

typedef struct s_vars
{
	int				i;
	int				j;
	int				n;
}					t_bvars;

typedef struct t_env
{
	char			*env_name;
	char			*env_value;
	struct t_env	*next;
}					t_env;

typedef struct s_filetype
{
	int				fd;
	char			*red;
	char			*type;
	char			*file_name;
	int				has_quotes;
}					t_filetype;

typedef struct s_list1
{
	void			*content;
	struct s_list1	*next;
}					t_list;

typedef struct s_cmds
{
	char			*cmd_name;
	char			**option;
	t_list			*files;
}					t_cmds;

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
	R_IN_OUT,
}					t_enum;

char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s1);
void				*ft_calloc(size_t count, size_t size);
void				ft_bzero(void *s, size_t n);
int					ft_strlen(const char *str);
void				*ft_memcpy(void *dest, const void *src, size_t n);
t_env				*get_env(char **env);
char				*get_env_value(char *env);
char				*get_env_name(char *env);
t_env				*ft_lstnew(char *name, char *value, t_env *new);
void				ft_lstadd_back(t_env **lst, t_env *new);
char				*add_spaces(char *input);
char				*ft_substr(char const *s, int start, int len);
char				*ft_strjoin(const char *s1, const char *s2);
int					calc_words(char *s, char c);
int					ft_wordlen(char *s, int i, char c);
char				*ft_charge(char *s, int i, char c, int flag);
void				*freeini(char **string, int len);
char				**ft_split(char *s, char sep);
int					ft_strncmp(const char *s1, const char *s2, int n);
int					ft_strcmp(char *s1, char *s2);
int					check_pipe(char *input);
int					check_redirec_op(char *input);
int					check_line(char *input);
char				*skip_spaces(char *input);
int					check_quotes(char *input);
int					check_outside(int count);
int					op_order(int *token);
int					is_outside(int flag, char c);
char				**quote_delete(char **cmd, int *delimiter, int *arr);
char				**expander(char **cmd, t_list *env, int *arr);
int					ft_envcmp(char *s1, char *s2, int length);
// t_list	**list_cmds(char **cmd_array, int *arr);
t_list				*my_lstnew(void *content);
void				my_lstadd_back(t_list **lst, t_list *new);
void				split_print(char **input);
void				print_list(t_list *list);
t_list				*list_cmds(char **cmd_array, int *arr, int *delimiter);
int					n_calculator(int num);
char				*ft_itoa(int num);
void				sig_handler();
void				handler(int sig);
int					check_delimiter(char *cmd);
// void				rl_replace_line(const char *text, int clear_undo);
// static char			*ft_expand(char *cmd, t_list *env, int *i);
// static char			*ft_expand_exit(char *cmd, t_list *env, int *i);
int					ft_isalnum(int i);
int					var_length(char *str);
char				*env_value(char *str, t_list *env);
int					ft_ischar(int i);

//	################		execution prototypes 		##################	//

//			String functions prototypes			//

int					ft_strcmpp(char *s1, char *s2);
char				*ft_strdupp(char *str);
int					ft_strlenn(char *str);
int					ft_strncmpp(const char *s1, const char *s2, size_t n);
char				*ft_strjoinn(char *s1, char *s2);
int					ft_atoi(char *str);
int					is_digit(char *str);
char				**ft_splitt(const char *s, char c);

//			Linked lists prototypes			//

t_list				*ft_lstneww(void *content);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_front(t_list **lst, t_list *new);
void				ft_lstadd_backk(t_list **lst, t_list *new);

//				Temporary				//

char				**dup_2d(char **str);
int					count_2d_len(char **str);

//				Pipex Prototypes		//

void				multiple_pipes(t_list *commands, t_list **env);
char				**get_path(char **envp);

//				Minishell				//

t_list				*make_env(char **envp);
int					execute_builtins(t_list *cmd, t_list **env);
int					execute_commands(t_list *cmd, t_list **env, char **args);
void				msg_exit(char *msg, char *msg1, int status);
char				**env_to_array(t_list **env);

char				**make_argv(t_list *commands);

int				open_file_type(t_filetype *files);
int				open_files(t_cmds *ptr);
void			dup_fds(t_cmds *ptr);
void			free_2d(char **array);
void			free_files(t_list *files);
void			my_free(t_list *commands);
void			shlvl_edit(t_list **env, int op);
void		here_docc(t_list *commands, t_list *env);
int	is_heredoc(t_list *commands);
void	free_int_arr(int **arr, int size);
void	close_files(t_list *commands);
int	is_space(char *str);

#endif