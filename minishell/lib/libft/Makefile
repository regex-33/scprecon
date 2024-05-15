# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yachtata <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/25 20:23:34 by yachtata          #+#    #+#              #
#    Updated: 2024/02/11 05:17:50 by yachtata         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CFLAGS = -g -Wall -Wextra -Werror
CC = cc
FT_PRINTF = ft_common_utils.c ft_print_utils.c ft_printf.c ft_flags_utils.c

LIBFT =  ft_strndup.c ft_strnlen.c ft_strjoin_free.c count_strings.c ft_strncpy.c ft_free_arr.c int_to_str.c ft_atoi_base.c ft_isupper.c ft_islower.c ft_isspace.c get_next_line.c get_next_line_utils.c ft_atoi.c ft_putchar.c ft_strcmp.c ft_putnbr.c ft_memmove.c ft_bzero.c ft_calloc.c ft_putstr.c ft_strcat.c ft_strcpy.c ft_print_int_array.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c ft_lstmap_bonus.c ft_lstiter_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstadd_back_bonus.c ft_lstlast_bonus.c ft_lstsize_bonus.c ft_lstadd_front_bonus.c ft_lstnew_bonus.c

.PHONY: all clean fclean re

RED			=	\033[0;31m
GREEN		=	\033[0;32m
NC			=	\033[0m

OBJSFD = objects

FT_PRINTF_SRCS = $(addprefix srcs/ft_printf/,$(FT_PRINTF))

LIBFT_SRCS = $(addprefix srcs/,$(LIBFT))

SRCS = $(LIBFT_SRCS) $(FT_PRINTF_SRCS)

OBJECTS = $(LIBFT:.c=.o) $(FT_PRINTF:.c=.o)

OBJS = $(addprefix $(OBJSFD)/, $(OBJECTS))

VPATH =	srcs:srcs/ft_printf

HDR = -I./includes

all: $(NAME)

$(NAME): $(OBJS)
	@ar src $@ $?

$(OBJSFD):
	@mkdir $@

$(OBJSFD)/%.o: %.c | $(OBJSFD)
	@$(CC) $(CFLAGS) $(HDR) -c $< -o $@

clean:
	@/bin/rm -f $(OBJS)
	@rm -rf $(OBJSFD)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all
