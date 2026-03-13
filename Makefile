TARGET = test_lexer

CC = gcc
CFLAGS = -Wall -Wextra -g -I.
SRCS = lexer.c tests/test-lexer.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean