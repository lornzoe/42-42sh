# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/05 18:24:23 by lyanga            #+#    #+#              #
#    Updated: 2026/08/05 18:24:24 by lyanga           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= 42sh

SRCS_DIR	= srcs
INCLUDES_DIR	= includes
OBJS_DIR	= .objs

SRCS		= $(SRCS_DIR)/main.c

OBJS		= $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
DEPS		= $(OBJS:.o=.d)

CC		= cc
CFLAGS		= -Wall -Wextra -Werror -MMD -MP -I$(INCLUDES_DIR)
LDFLAGS		= -lreadline

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
