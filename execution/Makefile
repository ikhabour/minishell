NAME = minishell
CC = cc
CFLAGS = -Wextra -Werror -Wall
SRCS = 	builtins.c\
		linked_list_utils.c\
		functions.c\
		ft_split.c\

OBJECTS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJECTS)
	$(CC) $(CFLAGS) $(SRCS) -lreadline -o $(NAME)

clean :
	rm -rf $(OBJECTS)

fclean : clean	
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re