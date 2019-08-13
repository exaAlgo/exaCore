# Compilers and flags
CC ?= mpicc
CFLAGS ?= -O2
CPPFLAGS ?=
LDFLAGS ?=

# Dependency locations
GSDIR ?=

# Build options
DEBUG ?= 1
SHARED ?= 0

# Install prefix
PREFIX ?= /home/thilina/local/

# Meta info about the package
SRCDIR ?= src
BUILDDIR ?= build
DEPDIR ?= .deps

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))
INCFLAGS = -I$(SRCDIR) -I$(GSDIR)/include

depflags.d = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
compile.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(INCFLAGS) -c

link.o = $(AR) crvs
LIBS = $(GSDIR)/lib/libgs.a
EXT = a

ifneq ($(SHARED),0)
  CFLAGS += -fPIC
  link.o += $(CC) $(LDFLAGS) -shared -o
  LIBS = -L$(GSDIR)/lib -lgs
  EXT = so
endif

$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	$(compile.c) $< -o $@

$(shell mkdir -p $(BUILDDIR))

.PHONY: lib
lib: $(OBJS)
	$(link.o) $(BUILDDIR)/libexa.$(EXT) $(OBJS) $(LIBS)

.PHONY: install
install:
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp $(SRCDIR)/*.h $(DESTDIR)$(PREFIX)/include/
	@cp $(BUILDDIR)/libexa.$(EXT) $(DESTDIR)$(PREFIX)/lib/

.PHONY: print
print :
	@echo $(VAR)=$($(VAR))
