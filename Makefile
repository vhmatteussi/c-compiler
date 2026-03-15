TARGET = test_lexer

CC = gcc
CFLAGS = -Wall -Wextra -g -Ilib/include -Isrc/include
SRCS = src/lexer.c \
       lib/alloc.c \
       lib/hash.c \
       lib/strings.c \
       lib/type.c \
       tests/test-lexer.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean