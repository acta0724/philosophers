# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/17 00:02:10 by iwasakatsuy       #+#    #+#              #
#    Updated: 2025/04/19 22:06:13 by kiwasa           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRCS =	src/main.c \
		src/init.c \
		src/routine.c \
		src/utils.c \
		src/destoroy_all.c \
		src/eat.c \
		src/sleep.c \
		src/think.c \
		src/monitor_death.c \
		src/create_threads.c \

OBJS = $(SRCS:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c inc/philo.h
	$(CC) $(CFLAGS) -I inc -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

h: re
	valgrind --tool=helgrind ./$(NAME) 5 800 200 200 3

p: $(NAME)
	./$(NAME) 4 310 200 200

test: $(NAME)
	./$(NAME) 5 800 200 200 5

.PHONY: all clean fclean re