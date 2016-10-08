EXEC = pipeprocsigaction

SRCDIR = src/
SRC = pipeprocsigaction.c
SRC := $(addprefix $(SRCDIR), $(SRC))

CC = gcc
CFLAGS = -W -Wall

LDFLAGS=
OBJ = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf $(SRC:.c=.o)

fclean: clean
	rm -rf $(EXEC)