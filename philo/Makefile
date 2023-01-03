# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/22 16:03:56 by lorbke            #+#    #+#              #
#    Updated: 2023/01/03 17:51:11 by lorbke           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# name macros
NAME = philo

# command macros
CC = cc -Wall -Wextra -Werror -g -pthread

# src and obj files macros
STRTOI_SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isspace.c ft_safeadd.c \
ft_safemult.c ft_strtoi.c
SRC = time.c philosophers.c philosophers_utils.c waiter.c philo.c
OBJ = $(SRC:.c=.o) $(STRTOI_SRC:.c=.o)

# default target
all: $(NAME)

# file targets
%.o: %.c
	$(CC) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

# phony targets
bonus: re

clean:
	@/bin/rm -f $(OBJ)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
