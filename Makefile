# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ikhabour <ikhabour@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/12 21:30:08 by bhazzout          #+#    #+#              #
#    Updated: 2023/06/23 02:57:25 by ikhabour         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

CFLAGS	=	-Wall -Wextra -Werror -Ireadline/8.2.1/include -fsanitize=address -g3 #-fno-omit-frame-pointer
 

RM		=	rm -rf

SOURCES	=	parsing/minishell.c parsing/env_processor.c parsing/helper/helper_1.c parsing/helper/helper_2.c parsing/helper/helper_4.c parsing/helper/helper_3_linked_list.c\
			parsing/line_processor.c parsing/split.c parsing/checker/check_pipe.c parsing/checker/op_checker.c parsing/checker/special_char_check.c\
			parsing/checker/quotes_check.c parsing/checker/order_checker.c parsing/quote_deleter.c parsing/quote_deleter_utils.c parsing/expand/expander.c parsing/expand/expander_utils.c\
			parsing/expand/expander_utils_1.c parsing/list_commands.c parsing/list_commands_utils.c\
			execution/builtins.c execution/ft_split.c execution/functions.c execution/linked_list_utils.c execution/commands.c\
			execution/multiple_pipes.c parsing/signals.c execution/here_docc.c execution/builtins1.c execution/builtins2.c\
			execution/builtins3.c execution/builtins4.c execution/builtins5.c execution/builtins6.c execution/builtins7.c\
			execution/builtins8.c execution/commands1.c execution/commands2.c execution/commands3.c execution/functions1.c\
			execution/here_docc1.c execution/here_docc2.c execution/multiple_pipes1.c execution/multiple_pipes2.c execution/multiple_pipes3.c\
			parsing/line_processor1.c parsing/minishell1.c parsing/minishell2.c parsing/minishell3.c parsing/minishell4.c\
			

OBJECTS	=	$(SOURCES:.c=.o)

all		:	$(NAME)

$(NAME) : 	$(OBJECTS)
	cc $(CFLAGS) $(OBJECTS) -o $(NAME) -lreadline  -L./readline/8.2.1/lib

clean	:
	$(RM) $(OBJECTS)

fclean	:	clean
	$(RM) $(NAME)

re		:	fclean all
