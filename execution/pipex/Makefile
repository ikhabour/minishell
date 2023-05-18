NAME = pipex
CFLAGS = -Werror -Wall -Wextra
CC = CC
SRCS =	pipex.c\
		pipex_utils.c\
		ft_split.c

OBJECTS = $(SRCS:.c=.o)

all :	$(NAME)

$(NAME) : $(OBJECTS)
	$(CC) $(SRCS) $(CFLAGS) -o $(NAME)

clean :
	rm -rf $(OBJECTS)
fclean : clean
	rm -rf $(NAME)
re : fclean clean all

.PHONY : all clean fclean re