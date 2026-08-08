# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/05 18:24:23 by lyanga            #+#    #+#              #
#    Updated: 2026/08/08 18:18:01 by lyanga           ###   ########.fr        #
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

# init script to download reference shells and extract them for testing
REFS_DIR	= .refs

init:
	git submodule update --init --remote --recursive
	rm -rf $(REFS_DIR)
	mkdir -p $(REFS_DIR)
	cd $(REFS_DIR) && apt-get download yash dash
	dpkg-deb -x $(REFS_DIR)/yash_*.deb $(REFS_DIR)/yash
	dpkg-deb -x $(REFS_DIR)/dash_*.deb $(REFS_DIR)/dash
	cp $(REFS_DIR)/yash/usr/bin/yash yash-reference
	cp $(REFS_DIR)/dash/bin/dash dash-reference
	rm -rf $(REFS_DIR)

.PHONY: all clean fclean re init
