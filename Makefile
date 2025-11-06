# requires GNU readline
# macOS: brew install readline
# Linux: sudo apt install libreadline-dev
UNAME_S := $(shell uname -s)

CFLAGS  = -Wall -Wextra -Werror
LIBS    = -lreadline

ifeq ($(UNAME_S), Darwin)
CFLAGS  += -I/usr/local/opt/readline/include -I/opt/homebrew/opt/readline/include
LDFLAGS += -L/usr/local/opt/readline/lib     -L/opt/homebrew/opt/readline/lib
else
# Linux
CFLAGS  +=
LDFLAGS +=
endif

OUT = minishell
SRC = srcs/builtins.c \
      srcs/cd.c \
      srcs/cd_utils.c \
      srcs/command_table.c \
      srcs/command_table_utils.c \
      srcs/echo.c \
      srcs/env.c \
      srcs/env_utils.c \
      srcs/exec_utils.c \
      srcs/expansion_utils.c \
      srcs/export_unset.c \
      srcs/h_doc.c \
      srcs/heredoc.c \
      srcs/heredoc_utils.c \
      srcs/main.c \
      srcs/main_utils.c \
      srcs/path.c \
      srcs/redirections.c \
      srcs/set_exit.c \
      srcs/signals.c \
      srcs/token_builtins.c \
      srcs/token_checks.c \
      srcs/token_list_utils.c \
      srcs/token_merge.c \
      srcs/token_splits.c \
      srcs/tokenizer.c \
      srcs/var_exapansion.c
OBJ = $(SRC:.c=.o)
CC  = cc

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(OUT)

re: fclean all
