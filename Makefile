# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/21 12:11:54 by bchanaa           #+#    #+#              #
#    Updated: 2024/05/18 20:30:32 by yachtata         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
# COLORS NOCOL=\033[0m
#RED=\033[1;31m
YEL=\033[1;33m
GRN=\033[1;32m
BLU=\033[1;34m


LYELLOW = \033[38;5;222m
BEIGE = \033[38;5;223m
LGREEN = \033[38;5;155m

BOLD = \033[1m
CUR = \033[3m
UL = \033[4m
UP = \033[A

# NAMES
NAME = scprecon 

# MAKE
MAKE = make

# COMPILER
CC = gcc

LFT_NAME = libft.a

# PATHS
INC_PATH = inc
SRC_PATH = src
OBJ_PATH = obj
LIB_PATH = lib

LFT_DIR = $(LIB_PATH)/libft

# LIBS
LFT = $(LFT_DIR)/$(LFT_NAME)

CFLAGS = -g -Wall -Wextra  -D BUFFER_SIZE=10
CFLAGS += -I ./$(LFT_DIR)/inc -Iinc -Iinc/readline

LDFLAGS = -L ./
LDLIBS = -lft -ltermcap -O3

# ANIMATION
SRCS_COUNT = 0
SRCS_TOT = ${shell find ./src/ -type f -name '*.c' | wc -l}
SRCS_PRCT = ${shell expr 100 \* ${SRCS_COUNT} / ${SRCS_TOT}}
#determine the length of the progress bar.
BAR =  ${shell expr 23 \* ${SRCS_COUNT} / ${SRCS_TOT}}


ifdef DEBUG
	CFLAGS += -fsanitize=address -g3
endif

# SOURCES
BUILTIN_SOURCES = main.c builtin_utils.c environment.c ft_cd_etc.c ft_change_dir.c ft_unset.c ft_pwd_and_env.c ft_exit.c ft_export.c ft_export_etc.c ft_echo.c 
PARSING_SOURCES = util_funcs.c lexer.c real_parser.c binary_tree.c parse_utils.c heredoc.c #exec.c
EXECUTION_SOURCES = pipes.c redirections.c exec.c etc_functions.c execut_cmd.c #parse_command.c
EXPANDING_SOURCES = heredoc_expanding.c expanding.c expanding_utils.c mereg_sort.c single_duble_dollar.c asterisk.c
SIGNALS_SOURCES = signals.c
RECON_SOURCES = creat_files.c creat_processes.c help.c parse_domainname.c resolve_domains.c compare_subdomains.c post_to_discord.c remove.c scprecon.c recon_utils.c
# PIPX_SOURCES = util_funcs.c lexer.c real_parser.c binary_tree.c exec.c


#SRC_FILES = main_bonus.c io_utils_bonus.c command_utils_bonus.c	

#BUILTIN_SRC = $(addprefix $(SRC_PATH)/builtin/, $(BUILTIN_SOURCES))
#PARSING_SRC = $(addprefix $(SRC_PATH)/parsing/, $(PARSING_SOURCES))
#PIPX_SRC = $(addprefix $(SRC_PATH)/pipx/, $(PIPX_SOURCES))

ALL_SOURCES = $(SIGNALS_SOURCES) $(BUILTIN_SOURCES) $(PARSING_SOURCES) $(EXECUTION_SOURCES) $(EXPANDING_SOURCES) $(RECON_SOURCES)
vpath %.c src/signals src/builtin/ src/parsing/ src/execution/ src/expanding/ src/recon/
vpath %.h inc
# OBJ_FILES = $(ALL_SOURCES:%.c=%.o)

OBJ_BUILTIN_FILES = $(BUILTIN_SOURCES:%.c=%.o)
OBJ_PARSING_FILES = $(PARSING_SOURCES:%.c=%.o)
OBJ_EXECUTION_FILES = $(EXECUTION_SOURCES:%.c=%.o)
OBJ_EXPANDING_FILES = $(EXPANDING_SOURCES:%.c=%.o)
OBJ_SIGNALS_FILES = $(SIGNALS_SOURCES:%.c=%.o)
OBJ_RECON_FILES = $(RECON_SOURCES:%.c=%.o)
# OBJ_PIPX_FILES = $(PIPX_SOURCES:%.c=%.o)

OBJ_FILES = $(addprefix $(OBJ_PATH)/, $(OBJ_SIGNALS_FILES) $(OBJ_BUILTIN_FILES) $(OBJ_PARSING_FILES) $(OBJ_EXECUTION_FILES) $(OBJ_EXPANDING_FILES) $(OBJ_RECON_FILES))

all: $(NAME)
$(NAME): $(LFT_NAME) $(OBJ_FILES)
	@echo "\n"
# @echo "i am here\n"
	@$(CC) $^ -o $@ $(CFLAGS) -L./lib/readline -lreadline -lcurl $(LDFLAGS)  $(LDLIBS) -fsanitize=address
# @echo "i am here\n"
	@echo "\n\n\n   ${BOLD}${CUR}${LYELLOW}MINISHELL COMPILED ✨${DEF}\n"

parser: $(LFT_NAME)
	cc -fsanitize=address -g3 $(PARSING_SRC) -Iinc -Llib/libft -lft

# #is used to redirect both standard output (stdout) and standard error (stderr) to /dev/null
$(LFT_NAME):
	@$(MAKE) all -sC $(LFT_DIR)	> /dev/null 2>&1
	@cp $(LFT) $(LFT_NAME)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH) > /dev/null 2>&1

$(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
#The eval command allows you to perform dynamic evaluation and assignment within the Makefile.
	@${eval SRCS_COUNT = ${shell expr ${SRCS_COUNT} + 1}}
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo ""
	@echo " ${BOLD}${CUR}${BEIGE}-> Compiling ${DEF}${BOLD}${LYELLOW}[MINISHELL]${DEF}"
	@printf " ${BEIGE}   [${LGREEN}%-23.${BAR}s${BEIGE}] [%d/%d (%d%%)]${DEF}" "***********************" ${SRCS_COUNT} ${SRCS_TOT} ${SRCS_PRCT}
#@echo "${UP}${UP}${UP}": Uses ANSI escape sequences (${UP}) to move the cursor up three lines
	@echo "${UP}${UP}${UP}"

debug:
	@echo "\n   ${BOLD}${UL}${MAGENTA}DEBUGGING MODE${DEF}"
	@${MAKE} DEBUG=1

clean:
	@$(MAKE) clean -sC $(LFT_DIR) > /dev/null 2>&1
	@rm -rf $(LFT_NAME)
	@rm -rf $(OBJ_PATH)

fclean: clean
	@$(MAKE) fclean -sC $(LFT_DIR) > /dev/null 2>&1
	@rm -rf $(NAME)

re: fclean all

install_go:
	@echo "\n${GRN}##########${YEL} INSTALLING GO ${GRN}##########${NOCOL}\n"
	@echo "${GRN}Downloading go...${NOCOL}"
	@sudo apt install golang-go
	@echo "${GRN}Setting up go...${NOCOL}"
	@echo "export PATH=$PATH:/usr/local/go/bin" >> ~/.bashrc
	@source ~/.bashrc

requirements:
	@echo "\n${GRN}##########${YEL} INSTALLING ${GRN}##########${NOCOL}\n"
	@echo "${GRN}Installing p...${NOCOL}"
	@go install github.com/d3mondev/puredns/v2@latest
	@echo "${GRN}Installing amass...${NOCOL}"
	@sudo apt install massdns


.PHONY: all clean fclean re norm
