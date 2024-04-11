CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm

# Lista de arquivos fonte
SRCS = $(wildcard *.c)

# Lista de arquivos objeto
OBJS = $(SRCS:.c=.o)

# Nome do executável
EXEC = mundo

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
