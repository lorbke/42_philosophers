# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/04 15:11:52 by lorbke            #+#    #+#              #
#    Updated: 2023/01/11 22:50:47 by lorbke           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# name macro
NAME = philo_bonus

# command macros
CC = gcc
RM = rm -f
ADD_FLAGS = -pthread #-g -fsanitize=address,undefined
FLAGS = -Wall -Wextra -Werror

# path macros
INC_PATH = includes
OBJ_PATH = obj
SRC_PATH = src
LIB_PATH = $(SRC_PATH)/ft_lib

# src and obj files macros
SRC = ft_isalnum.c ft_isalpha.c ft_isdigit.c ft_isspace.c ft_safeadd.c \
ft_safemult.c ft_strisdigit.c ft_strtoi.c main.c parse.c philo_utils.c \
philo.c time.c waitress.c
OBJ = $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# default rule
default: makedir all

# file targets
${NAME}: ${OBJ}
	${CC} ${FLAGS} ${ADD_FLAGS} ${OBJ} -o ${NAME}
	@echo "make: philo_bonus success!"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	${CC} ${FLAGS} ${ADD_FLAGS} -I$(INC_PATH) -c $< -o $@

# phony targets
all: ${NAME}

makedir:
	@mkdir -p $(OBJ_PATH)

clean:
	${RM} -r $(OBJ_PATH)

fclean: clean
	@${RM} ${NAME}

re: fclean makedir all

.PHONY: all clean fclean re