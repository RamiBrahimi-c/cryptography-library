# ============================================================================
# Configuration
# ============================================================================
CC       := gcc
AR       := ar
CFLAGS   := -Wall -Wextra -fPIC
CPPFLAGS := -Iinclude -Iinclude/ciphers/classical -Iinclude/ciphers/symmetric \
            -Iinclude/ciphers/asymmetric -Iinclude/ciphers/hashing

# bignum library
BIGNUM_DIR     := ./third-party/big-ar9am
BIGNUM_LIB     := $(BIGNUM_DIR)/lib/libbigra9m.a
CPPFLAGS       += -I$(BIGNUM_DIR)/include
LDFLAGS        := -L$(BIGNUM_DIR)/lib
LDLIBS         := -lbigra9m -lm

# Tests get GMP on top for verification purposes
TEST_LDLIBS    := $(LDLIBS) -lgmp

# ============================================================================
# Directory structure
# ============================================================================
SRCDIR   := src
OBJDIR   := obj
BINDIR   := bin
LIBDIR   := lib
TESTDIR  := tests
TESTOUT  := $(TESTDIR)/output/bin

# ============================================================================
# Source discovery
# ============================================================================
COMMON_SRC     := $(wildcard $(SRCDIR)/common/*.c)
CLASSICAL_SRC  := $(wildcard $(SRCDIR)/ciphers/classical/*.c)
SYMMETRIC_SRC  := $(wildcard $(SRCDIR)/ciphers/symmetric/*.c)
ASYMMETRIC_SRC := $(wildcard $(SRCDIR)/ciphers/asymmetric/*.c)
HASHING_SRC    := $(wildcard $(SRCDIR)/ciphers/hashing/*.c)

# Full library sources
FULL_SRC := $(COMMON_SRC) $(CLASSICAL_SRC) $(SYMMETRIC_SRC) \
            $(ASYMMETRIC_SRC) $(HASHING_SRC)

# Lite library sources (no asymmetric, no hashing, no bignum)
LITE_SRC := $(COMMON_SRC) $(CLASSICAL_SRC) $(SYMMETRIC_SRC)

# Object files
FULL_OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/full/%.o,$(FULL_SRC))
LITE_OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/lite/%.o,$(LITE_SRC))

# ============================================================================
# Phony targets
# ============================================================================
.PHONY: all clean deps chat server client crypto check-bignum

# ============================================================================
# Preflight checks
# ============================================================================
check-bignum:
	@if [ ! -f $(BIGNUM_LIB) ]; then \
		echo "*** libbigra9m.a not found at $(BIGNUM_LIB)"; \
		echo "*** Please clone and build big-ra9am first, or drop a prebuilt"; \
		echo "*** libbigra9m.a into $(BIGNUM_DIR)/lib/"; \
		exit 1; \
	fi

# ============================================================================
# Default target
# ============================================================================
all: $(LIBDIR)/libsifra.a $(LIBDIR)/libsifra.so \
     $(LIBDIR)/libsifra-lite.a $(LIBDIR)/libsifra-lite.so

# ---------------------------------------------------------------------------
# Full static library (no bignum needed — just an archive of .o files)
# ---------------------------------------------------------------------------
$(LIBDIR)/libsifra.a: $(FULL_OBJ)
	@mkdir -p $(LIBDIR)
	$(AR) rcs $@ $^

# ---------------------------------------------------------------------------
# Full shared library (absorbs libbigra9m.a)
# ---------------------------------------------------------------------------
$(LIBDIR)/libsifra.so: check-bignum $(FULL_OBJ) $(BIGNUM_LIB)
	@mkdir -p $(LIBDIR)
	$(CC) -shared -o $@ $(FULL_OBJ) $(LDFLAGS) $(LDLIBS)

# ---------------------------------------------------------------------------
# Lite static library
# ---------------------------------------------------------------------------
$(LIBDIR)/libsifra-lite.a: $(LITE_OBJ)
	@mkdir -p $(LIBDIR)
	$(AR) rcs $@ $^

# ---------------------------------------------------------------------------
# Lite shared library (no bigra9m)
# ---------------------------------------------------------------------------
$(LIBDIR)/libsifra-lite.so: $(LITE_OBJ)
	@mkdir -p $(LIBDIR)
	$(CC) -shared -o $@ $(LITE_OBJ) -lm

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
# Main executable
# ============================================================================
crypto: check-bignum $(BINDIR)/crypto

$(BINDIR)/crypto: $(SRCDIR)/main.c $(LIBDIR)/libsifra.a
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIBDIR)/libsifra.a $(LDFLAGS) $(LDLIBS) -o $@

# ============================================================================
# Chat application
# ============================================================================
chat: check-bignum $(BINDIR)/server $(BINDIR)/client

server: check-bignum $(BINDIR)/server
client: check-bignum $(BINDIR)/client

$(BINDIR)/server: $(SRCDIR)/server.c $(LIBDIR)/libsifra.a
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIBDIR)/libsifra.a $(LDFLAGS) $(LDLIBS) -o $@

$(BINDIR)/client: $(SRCDIR)/client.c $(LIBDIR)/libsifra.a
	@mkdir -p $(BINDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIBDIR)/libsifra.a $(LDFLAGS) $(LDLIBS) -o $@

# ============================================================================
# Tests / with gmp tests
# ============================================================================
test_%: $(TESTDIR)/test_%.c $(LIBDIR)/libsifra.a check-bignum
	@mkdir -p $(TESTOUT)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIBDIR)/libsifra.a $(LDFLAGS) $(LDLIBS) -o $(TESTOUT)/$@
	$(TESTOUT)/$@

test_gmp_%: $(TESTDIR)/asymmetric-gmp/test_%.c $(LIBDIR)/libsifra.a check-bignum
	@mkdir -p $(TESTOUT)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIBDIR)/libsifra.a $(LDFLAGS) $(TEST_LDLIBS) -o $(TESTOUT)/$@
	$(TESTOUT)/$@

# ============================================================================
# Utility targets
# ============================================================================
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(LIBDIR) $(TESTDIR)/output

deps:
	$(CC) $(CPPFLAGS) -MM $(FULL_SRC) > .depend

-include .depend