/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 15:27:28 by ikhabour          #+#    #+#             */
/*   Updated: 2023/05/17 20:51:14 by ikhabour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_builtinvars
{
	int				n;
	int				i;
	int				j;
}					t_bvars;

typedef struct s_filetype
{
	char			*type;
	char			*file_name;
}					t_filetype;

typedef struct s_cmds
{
	char			*cmd_name;
	char			**option;
	t_filetype		files;
}					t_cmds;

typedef struct s_list1
{
	void			*content;
	struct s_list1	*next;
}					t_list;

//			String functions prototypes			//

int					ft_strcmp(char *s1, char *s2);
char				*ft_strdupp(char *str);
int					ft_strlenn(char *str);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strjoinn(char *s1, char *s2);
int					ft_atoi(char *str);
int					is_digit(char *str);
char				**ft_split(const char *s, char c);

//			Linked lists prototypes			//

t_list				*ft_lstnew(void *content);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_front(t_list **lst, t_list *new);
void				ft_lstadd_back(t_list **lst, t_list *new);

//				Temporary				//

char				**dup_2d(char **str);
int					count_2d_len(char **str);

#endif