CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Iinclude/ciphers/classical -Iinclude/ciphers/symmetric
LDFLAGS = -lm
SRCDIR = src
OBJDIR = obj

# Find all .c files recursively
SOURCES = $(shell find $(SRCDIR) -type f -name '*.c')
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Main target
crypto: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

# Compile rule
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJDIR) crypto run_tests

# Test - includes all cipher source files recursively
test:
	$(CC) $(CFLAGS) tests/test_*.c $(shell find src/ciphers -type f -name '*.c') src/common/*.c $(LDFLAGS) -o run_tests && ./run_tests

# Run specific test
test-%:
	$(CC) $(CFLAGS) tests/test_$*.c $(shell find src/ciphers -type f -name '*.c') src/common/*.c $(LDFLAGS) -o run_tests && ./run_tests

.PHONY: clean test