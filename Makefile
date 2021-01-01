# SRC = $(wildcard *.c) $(wildcard */*.c)
NAME = libras

CC = gcc
FLAGS = -Wall -Wextra -Wundef -Wshadow -Wunreachable-code
USE_OBJ = 0

SRC = main.c utils.c common.c menu/login.c menu/user.c
OBJ = $(SRC:.c=.o)

ifeq ($(OS), Windows_NT)
DEL = del
else
DEL = rm -f
endif

# ----------------------------------------------------- #

# Handles complete compiling
ifeq ($(USE_OBJ), 1)
all: $(OBJ)
	$(CC) $(FLAGS) $(OBJ:menu/%=%) -o $(NAME)
else
all:
	$(CC) $(FLAGS) $(SRC) -o $(NAME)
	.\libras.exe
endif

# Handles pre-compiling the files if needed
ifeq ($(USE_OBJ), 1)
%.o: %.c
	$(CC) $(FLAGS) -c $^
endif

# ----------------------------------------------------- #

# Handles cleaning the files 
.PHONY: clean
clean:
	$(DEL) $(wildcard *.o) $(wildcard *.exe)
	cls