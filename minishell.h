/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 15:36:24 by bhazzout          #+#    #+#             */
/*   Updated: 2023/06/23 02:58:55 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/errno.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_sig
{
	int				exit_s;
	int				execc;
	int				readline;
	int				process;
}					t_sig;

extern t_sig		g_sigs;

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

typedef struct s_variables
{
	int				delimiter;
	char			**cmd_array;
	char			**new;
	int				*arr;
	t_list			*commands;
	t_list			*tmp;
	int				arg_counter;
	t_cmds			*node;
	t_list			*file;
	t_filetype		*file_node;
}					t_vars;

typedef struct s_exec
{
	int				pid;
	int				i;
	char			*f_path;
}					t_exec;

typedef struct s_pipes
{
	int				pipes;
	int				**fd;
	int				*pids;
	int				status;
}					t_pipes;

typedef struct s_spipes
{
	char			**argv;
	char			**paths;
	char			*f_path;
	char			**envp;
}					t_spipes;

typedef struct s_heredoc
{
	int				pid;
	int				**fds;
	int				docs;
	int				i;
	t_list			*tmp;
}					t_heredoc;

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
// char				*skip_spaces(char *input);
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
void				sig_handler(void);
void				handler(int sig);
int					check_delimiter(char *cmd);
// void				rl_replace_line(const char *text, int clear_undo);
// static char			*ft_expand(char *cmd, t_list *env, int *i);
// static char			*ft_expand_exit(char *cmd, t_list *env, int *i);
int					ft_isalnum(int i);
int					var_length(char *str);
char				*env_value(char *str, t_list *env);
int					ft_ischar(int i);
char				*ft_strtrim(char *s1, char *set);
void				start(char *input);
void				mid(char *input);
int					end(char *input, int i);
int					op_start(char *input, int i);
int					op_end(char *input, int i);
void				d_quoted(char *str, char *cmd, int *i, int *j);
void				s_quoted(char *str, char *cmd, int *i, int *j);
int					limiter_value(int *i, char *cmd);
int					start_index(char *cmd, int *i);
char				*expand_processor(char *cmd, t_list *env);
int					ft_isalnum(int i);
int					var_length(char *str);
char				*env_value(char *str, t_list *env);
int					ft_ischar(int i);
char				*cmd_expand(char *cmd, int limiter, char *lineup,
						char *full_str);
void				split_print(char **input);
void				print_list(t_list *list);
void				array_printer(int *input);
void				node_printer(t_list *file_node);
void				fill_in_out(char **cmd_array, int i, t_filetype *file_node);
void				fill_out_file(char **cmd_array, int i,
						t_filetype *file_node);
void				fill_app_file(char **cmd_array, int i,
						t_filetype *file_node);
void				fill_in_file(char **cmd_array, int i,
						t_filetype *file_node);
t_filetype			*fill_file(char **cmd_array, int *arr, int i,
						int *delimiter);
void				sig_quit(int sig);

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

int					open_file_type(t_filetype *files);
int					open_files(t_cmds *ptr);
void				dup_fds(t_cmds *ptr);
void				free_2d(char **array);
void				free_files(t_list *files);
void				my_free(t_list *commands);
void				shlvl_edit(t_list **env, int op);
void				here_docc(t_list *commands, t_list *env);
int					is_heredoc(t_list *commands);
void				free_int_arr(int **arr, int size);
void				pipes_helper(int i, t_pipes var);
void				pipes_helper1(int i, t_list *tmp, t_pipes var,
						t_list **env);
void				pipes_helper2(int i, t_pipes var, t_list **tmp);
void				close_files(t_list *commands);
int					is_space(char *str);

int					exec_builtins_pt1(t_list *cmd, t_cmds *tmp, t_list **env,
						int fd);
int					exec_builtins_pt2(t_list *cmd, t_cmds *tmp, t_list **env,
						int fd);
int					execute_builtins(t_list *cmd, t_list **env);

int					check_echo_option(char *str, t_bvars *var);
void				init_bvars(t_bvars *var);
void				echo_helper(t_bvars var, t_cmds *tmp, int out_fd);
void				echo_helper2(t_bvars var, t_cmds *tmp);
void				execute_echo(t_list *cmd);

t_list				*make_env(char **envp);
int					change_value(char *argument);
void				execute_env(t_list **env, t_list *cmd);
int					append_value(char *argument);
char				*remove_plus(char *string);

void				print_string(char *str);
void				print_env(t_list **env);
void				print_export(t_list **env);
int					var_exists(char *var, t_list **env);
int					is_alpha(char c);

int					unset_valid(char *str);
int					valid_identifier(char *str);
void				identifier_error(char *option);
void				export_helper(t_bvars var, t_cmds *ptr, t_list *tmp,
						t_list **env);
void				export_helper2(t_bvars var, t_list *tmp, t_cmds *ptr,
						t_list **env);

void				export_helper3(t_bvars var, t_list **tmp, t_list **env,
						t_cmds *ptr);
int					export_and_files(t_cmds *ptr, t_list **env, int fd);
void				export_while(t_cmds *ptr, t_bvars var, t_list *tmp,
						t_list **env);
void				execute_export(t_list *cmd, t_list **env);
int					unset_helper(t_list **curr, t_cmds *ptr, t_list **env,
						int i);

int					unset_helper1(t_list **curr, t_list **prev, int fd);
void				sub_while(t_list **curr, t_cmds *ptr, int i, t_list **prev);
int					unset_while(t_cmds *ptr, t_list *curr, t_list *prev,
						t_list **env);
void				execute_unset(t_list *cmd, t_list **env);
void				write_exit(void);

void				write_and_exit(int exit_status, t_list **env);
void				check_exit_status(int exit_status, t_list **env);
void				check_exit_errors(t_cmds *ptr, t_list **env);
void				execute_exit(t_list *cmd, t_list **env);
void				exportt(char *value, char *var, t_list **env);

void				setpwd(t_list **env);
int					check_for_permission(t_cmds *ptr, int fd);
int					find_home(t_list *tmp, int fd);
void				chdir_and_pwd(char *dir, int fd, t_list **env);
void				execute_cd(t_list *cmd, t_list **env);

void				cmd_checks(t_cmds *ptr);
void				cmds_while(t_exec *vars, char **paths, t_cmds *ptr);
void				cmds_free(char **envp, char **args);
void				cmd_helper(t_cmds *ptr, char **args, char **envp,
						t_exec vars);
int					execute_commands(t_list *cmd, t_list **env, char **args);

int					count_len(t_list **env);
char				**env_to_array(t_list **env);
char				**get_path(char **envp);
int					get_len(char **arg);
void				msg_exit(char *msg, char *msg1, int status);

int					open_files(t_cmds *ptr);
int					dup_helper(t_filetype *files, int *out, int *in);
void				dup_fds(t_cmds *ptr);
int					has_redirection(char **args);
void				wait_process(t_exec *vars);

int					open_file_type(t_filetype *files);
int					ft_atoi(char *str);
int					is_digit(char *str);
int					count_2d_len(char **str);
char				**dup_2d(char **str);

int					ft_strlenn(char *str);
int					ft_strcmpp(char *s1, char *s2);
char				*ft_strdupp(char *str);
int					ft_strncmpp(const char *s1, const char *s2, size_t n);
char				*ft_strjoinn(char *s1, char *s2);

void				close_files(t_list *commands);
int					is_heredoc(t_list *commands);
void				ft_putstr_fd(char *s, int fd);
int					limiter_value_ex(int *i, char *cmd);
char				*cmd_expand_ex(char *cmd, int limiter, char *lineup,
						char *full_str);

void				doc_child_process(t_filetype *p, t_heredoc var,
						t_list *env);
void				here_doc_helper(t_heredoc var, t_filetype *p,
						t_filetype **last_heredoc);
void				count_docs(t_heredoc *var, t_filetype **last_heredoc,
						t_filetype **p);
void				here_doc_helper1(t_heredoc *var, t_cmds *ptr);
void				here_docc(t_list *commands, t_list *env);

int					start_index_ex(char *cmd, int *i);
// static char			*ft_expand_ex(char *cmd, t_list *env, int *i);
// static char			*ft_expand_exit_ex(char *cmd, t_list *env, int *i);
char				*expand_heredoc(char *input, t_list *env);
void				display_prompt(t_list *files, int fd, t_list *env);

int					count_pipes(t_list *commands);
char				**make_argv(t_list *commands);
int					input_file(t_cmds *ptr);

void				first_command(t_list *commands, t_list **env, int *fd);
void				last_command(t_list *commands, t_list **env, int *fd);
void				subpipes_helper3(t_cmds *ptr, int *fdout, int *fdin);
void				subpipes_helper4(t_cmds *ptr, t_list *commands,
						t_list **env, t_spipes var);
void				middle_command(t_list *commands, t_list **env, int *fdin,
						int *fdout);
void				dup_input_file(t_cmds *ptr);
int					output_file(t_cmds *ptr);
void				dup_output_file(t_cmds *ptr);
void				subpipes_helper(t_cmds *ptr, t_list *commands, t_list **env,
						int *fd);
void				subpipes_helper2(t_cmds *ptr, t_list *commands,
						t_list **env, int *fd);

void	*add_space_before(char *input, int i);
void	*add_space_after(char *input, int i);
char	*process_before(char *input, int i, int *flag);
char	*process_after(char *input, int i, int *flag);
char	*add_spaces(char *input);

char	*input_get(char *input);
int	command_valid(t_vars var, char *input);
void	my_allfree(char *input, t_vars var);
void	close_free(t_vars var, char *input);
int	executer(t_vars var, char *input, t_list **env);

int	valid_command(char **new);
char	*signal_handler(char *input, t_list *commands);
int	add_free(char *input);
int	input_checker(char *input);
int	order_checker(int *arr, char *input, char **cmd_array);

int	is_space(char *str);
void	free_files(t_list *files);
void	my_free(t_list *commands);
void	free_2d(char **array);
void	free_all(char *input, char **array);

char	*fill_line(char *input);
int	name_arg(int *cmd_token, char **cmd_array, int i, int *flag);
int	enum_token(int *cmd_token, char **cmd_array, int i, int *flag);
int	*array_tokens(char **cmd_array, int elements);
int	num_elemnts(char **cmd_array);

int	space_before(char *input, int i);
int	space_after(char *input, int i);
#endif