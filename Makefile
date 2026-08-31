# ============================================================================
# Configuration
# ============================================================================
CC       := gcc
AR       := ar
CFLAGS   := -Wall -Wextra -lm -lgmp
CPPFLAGS := -Iinclude -Iinclude/ciphers/classical -Iinclude/ciphers/symmetric \
            -Iinclude/ciphers/asymmetric -Iinclude/ciphers/hashing

# Your bignum library
BIGNUM_DIR     := ./third-party/big-ar9am
CPPFLAGS       += -I$(BIGNUM_DIR)/include
LDFLAGS        := -L$(BIGNUM_DIR)/lib -lm
LDLIBS         := -lbigra9m

# ============================================================================
# Directory structure
# ============================================================================
SRCDIR   := src
OBJDIR   := obj
BINDIR   := bin
LIBDIR   := lib
TESTDIR  := tests

# ============================================================================
# Source discovery
# ============================================================================
# Split sources by category for better control
COMMON_SRC     := $(wildcard $(SRCDIR)/common/*.c)
CLASSICAL_SRC  := $(wildcard $(SRCDIR)/ciphers/classical/*.c)
SYMMETRIC_SRC  := $(wildcard $(SRCDIR)/ciphers/symmetric/*.c)
ASYMMETRIC_SRC := $(wildcard $(SRCDIR)/ciphers/asymmetric/*.c)
HASHING_SRC    := $(wildcard $(SRCDIR)/ciphers/hashing/*.c)

# Full library sources
FULL_SRC := $(COMMON_SRC) $(CLASSICAL_SRC) $(SYMMETRIC_SRC) \
            $(ASYMMETRIC_SRC) $(HASHING_SRC)

# Lite library sources (no GMP/OpenSSL dependency)
LITE_SRC := $(COMMON_SRC) $(CLASSICAL_SRC) $(SYMMETRIC_SRC)

# Object files
FULL_OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/full/%.o,$(FULL_SRC))
LITE_OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/lite/%.o,$(LITE_SRC))

# ============================================================================
# Targets
# ============================================================================
.PHONY: all clean test chat server client

all: $(LIBDIR)/libcrypto.a $(LIBDIR)/libcrypto-lite.a

# Full static library
$(LIBDIR)/libcrypto.a: $(FULL_OBJ)
	@mkdir -p $(LIBDIR)
	$(AR) rcs $@ $^

# Lite static library
$(LIBDIR)/libcrypto-lite.a: $(LITE_OBJ)
	@mkdir -p $(LIBDIR)
	$(AR) rcs $@ $^

# ============================================================================
# Compilation rules
# ============================================================================
$(OBJDIR)/full/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/lite/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# ============================================================================
# Tests (link against static library instead of recompiling)
# ============================================================================
TEST_SOURCES := $(wildcard $(TESTDIR)/test_*.c)
TEST_TARGETS := $(patsubst $(TESTDIR)/test_%.c,test_%,$(TEST_SOURCES))

test: $(TEST_TARGETS)

test_%: $(TESTDIR)/test_%.c $(LIBDIR)/libcrypto.a
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIBDIR)/libcrypto.a $(LDFLAGS) $(LDLIBS) -o $(BINDIR)/$@
	$(BINDIR)/$@

# ============================================================================
# Chat application
# ============================================================================
chat: $(BINDIR)/server $(BINDIR)/client

$(BINDIR)/server: $(SRCDIR)/server.c $(LIBDIR)/libcrypto.a
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIBDIR)/libcrypto.a $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/client: $(SRCDIR)/client.c $(LIBDIR)/libcrypto.a
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIBDIR)/libcrypto.a $(LDFLAGS) $(LDLIBS) -o $@

# ============================================================================
# Utility targets
# ============================================================================
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR)

# Dependency generation 
deps:
	$(CC) $(CPPFLAGS) -MM $(FULL_SRC) > .depend

-include .depend