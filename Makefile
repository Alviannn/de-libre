# SRC = $(wildcard *.c) $(wildcard */*.c)
NAME = de-libre
SRC = common.c utils.c menu/login.c menu/user.c menu/admin.c main.c

CC = gcc
FLAGS = -Wall -Wextra -Wundef -Wshadow -Wunreachable-code

ifeq ($(OS), Windows_NT)
DEL = del
else
DEL = rm -f
endif

# ----------------------------------------------------- #

# Handles complete compiling
all:
	$(CC) $(FLAGS) $(SRC) -o $(NAME)

# ----------------------------------------------------- #

# Handles cleaning the files 
.PHONY: clean
clean:
	$(DEL) $(wildcard *.o) $(wildcard *.exe)
	cls