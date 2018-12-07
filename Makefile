# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ktlili <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/07/10 23:32:37 by ktlili            #+#    #+#              #
#    Updated: 2018/10/06 19:30:38 by ktlili           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC_PATH = src

PARSER_PATH = src/parsing

SRC_NAME =	main.c \
			util.c \
			util2.c \
			testing.c \
			clean_path.c \
			execute.c \
			changedir.c \
			spawn_bin.c \
			env1.c \
			env_util.c \
			ft_env.c \
			expand_tokens.c \

SRC_PARSER = build_tree.c \
			 tree_tools.c \
		  	 eval.c \
			 syntax.c

OBJ_PATH = obj

OBJ_NAME = $(SRC_NAME:.c=.o) $(SRC_PARSER:.c=.o)

LIB = ./lib/libft.a

FLAGS = -ggdb -Wall -Wextra -Werror

SRC = $(addprefix $(SRC_PATH)/,$(SRC_NAME))
PARSER_SRC = $(addprefix $(PARSER_PATH)/,$(SRC_PARSER))


OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all:	$(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C ./lib/
	clang $(OBJ) $(LIB) -o $@

$(OBJ_PATH)/%.o: $(PARSER_PATH)/%.c 
	mkdir -p $(OBJ_PATH)
	clang -I. $(FLAGS) -o $@ -c $<

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c 
	mkdir -p $(OBJ_PATH)
	clang $(FLAGS) -o $@ -c $< -I.


clean:
	$(MAKE) -C ./lib/ clean
	/bin/rm -f $(OBJ)
	/bin/rm -rf $(OBJ_PATH)

fclean: clean
	$(MAKE) -C ./lib/ fclean
	/bin/rm -f $(NAME)

re: fclean all
