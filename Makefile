EXE     = bfip

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra

LD      = gcc

SRC    := $(wildcard *.c)
DEPS   := $(wildcard *.h)
OBJ    := $(SRC:.c=.o)

all: $(EXE)

$(EXE): $(OBJ)
	$(LD) $^ -o $(EXE)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(EXE)

.PHONY: $(EXE) clean install uninstall
