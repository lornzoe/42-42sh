# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/05 18:24:23 by lyanga            #+#    #+#              #
#    Updated: 2026/08/08 15:37:00 by lyanga           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= 42sh

SRCS_DIR	= srcs
INCLUDES_DIR	= includes
OBJS_DIR	= .objs

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a
LIBFT_HEADERS	= $(LIBFT_DIR)/headers

SRCS_FILES	=	main.c \
				tokeniser.c \
				token.c

SRCS		= $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))

OBJS		= $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

CC		= cc
CFLAGS		= -Wall -Wextra -Werror -std=gnu17 -MMD -MP -I$(INCLUDES_DIR) -I$(LIBFT_HEADERS)
LDFLAGS		= -lreadline

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

init:
	git submodule update --init --remote --recursive

.PHONY: all clean fclean re init
