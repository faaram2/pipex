# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ayeganya <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/19 11:05:36 by ayeganya          #+#    #+#              #
#    Updated: 2024/05/26 15:21:43 by ayeganya         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and compiler CFLAGS
CC = cc
CFLAGS = -Wall -Werror -Wextra #-g3 -fsanitize=address

# Header files
H_FILE = pipex.h

#Source files
SRC_FILES = pipex.c utils.c files_proc.c proc_starter.c 
#B_SRC_FILES = 

OBJ_FILES = $(SRC_FILES:%.c=%.o)

#Libft
LIBFT_DIR = ./libft
LIBFT_A = $(LIBFT_DIR)/libft.a


#Target
NAME  = pipex
BONUS = pipex

all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJ_FILES) $(H_FILE)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBFT_A) -o $@

#Object target
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

#Libft target
$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

bonus: $(LIBFT_A) $(NAME) 

#cleaning targets
clean:
	rm -f $(OBJ_FILES)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	#rm -f $(BONUS)

re: fclean all

.PHONY: all clean fclean re

