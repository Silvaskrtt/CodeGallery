CC = gcc
CFLAGS = -Iinclude -Wall -std=c11
LDFLAGS = -lsqlite3

SRC = src/database.c src/main.c
OBJ = $(SRC:.c=.o)
EXEC = meu_projeto

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)