# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/22 16:03:56 by lorbke            #+#    #+#              #
#    Updated: 2023/01/02 19:38:29 by lorbke           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = cc -Wall -Wextra -Werror

STRTOI_SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isspace.c ft_safeadd.c \
ft_safemult.c ft_strtoi.c
SRC = time.c routine.c waiter.c philo.c
OBJ = $(SRC:.c=.o) $(STRTOI_SRC:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

bonus: re

clean:
	@/bin/rm -f $(OBJ)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
