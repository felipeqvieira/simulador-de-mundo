CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm

# Lista de arquivos fonte
SRCS = $(wildcard src/*.c)

# Lista de arquivos objeto (todos dentro de src, exceto main.o)
OBJS = $(patsubst src/%.c, src/%.o, $(filter-out src/main.c, $(SRCS)))

# Nome do executável
EXEC = mundo

all: $(EXEC)

$(EXEC): $(OBJS) main.o
	$(CC) $^ -o $@ $(LDFLAGS)

src/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) main.o $(EXEC)
