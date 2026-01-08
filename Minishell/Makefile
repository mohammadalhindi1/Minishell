# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malhendi <malhendi@student.42amman.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/03 20:38:42 by malhendi          #+#    #+#              #
#    Updated: 2025/12/23 14:08:14 by malhendi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME      = minishell

CC        = cc
CFLAGS    = -Wall -Wextra -Werror -g

SRC_DIR   = src
OBJ_DIR   = obj

INC       = -I$(SRC_DIR)/includes -I$(SRC_DIR)/parsing -I$(SRC_DIR)/libft/includes

READLINE  = -lreadline -lhistory -lncurses

LIBFT_DIR = $(SRC_DIR)/libft
LIBFT     = $(LIBFT_DIR)/libft.a

RM        = rm -rf

GREEN     = \033[0;32m
RED       = \033[0;31m
RESET     = \033[0m

SRCS = \
	$(SRC_DIR)/minishell.c \
	$(SRC_DIR)/execution/execute_pipeline.c \
	$(SRC_DIR)/execution/execute_pipeline_utils.c \
	$(SRC_DIR)/execution/child.c \
	$(SRC_DIR)/execution/child_exec.c \
	$(SRC_DIR)/execution/builtins.c \
	$(SRC_DIR)/execution/exec_utils.c \
	$(SRC_DIR)/execution/builtins_helper.c \
	$(SRC_DIR)/execution/redir.c \
	$(SRC_DIR)/execution/heredoc.c \
	$(SRC_DIR)/execution/path.c \
	$(SRC_DIR)/execution/global.c \
	$(SRC_DIR)/parsing/minishell.c \
	$(SRC_DIR)/parsing/token.c \
	$(SRC_DIR)/parsing/valid_check.c \
	$(SRC_DIR)/parsing/expand.c \
	$(SRC_DIR)/helper/free.c \
	$(SRC_DIR)/helper/slvl.c \
	$(SRC_DIR)/helper/slvl2.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(INC) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)
	@echo "$(GREEN)Done ✔$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@$(MAKE) clean -C $(LIBFT_DIR)
	@echo "$(RED)Deleting $(OBJ_DIR)...$(RESET)"
	@$(RM) $(OBJ_DIR)
	@echo "$(RED)Done ✔$(RESET)"

fclean: clean
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@echo "$(RED)Deleting $(NAME)...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(RED)Done ✔$(RESET)"

re: fclean all

.PHONY: all clean fclean re
