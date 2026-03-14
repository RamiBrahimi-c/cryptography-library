CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRCDIR = src
INCDIR = include
OBJDIR = obj

SOURCES = $(shell find $(SRCDIR) -name '*.c')
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

crypto: $(OBJECTS)
	$(CC) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) crypto

test:
	$(CC) $(CFLAGS) tests/test_*.c src/ciphers/*.c src/common/*.c -o run_tests && ./run_tests
.PHONY: clean test