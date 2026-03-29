TARGET = main
CC = gcc
CFLAGS = -Wall -Wextra -g -Ilib/include -Isrc/include
SRCS = src/lexer.c \
       lib/alloc.c \
       lib/strings.c \
       lib/type.c \
       src/main.c

OBJS = $(SRCS:.c=.o)
all: $(TARGET)
src/keywords.c: lib/include/keywords.gperf
	gperf -t -N lookup_keyword $< > $@
src/lexer.o: src/lexer.c src/keywords.c
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean