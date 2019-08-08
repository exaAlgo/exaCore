CC ?= mpicc
CFLAGS ?= -O2
CPPFLAGS ?=
GSDIR ?=
LDFLAGS ?=
DEBUG ?= 1

SRCDIR ?= src
BUILDDIR ?= build
DEPDIR ?= .deps

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))
INCFLAGS = -I$(SRCDIR) -I$(GSDIR)/include

depflags.d = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
compile.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(INCFLAGS) -c
link.o = $(CC) $(LDFLAGS)

$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	$(compile.c) $< -o $@

$(shell mkdir -p $(BUILDDIR))

all: $(OBJS)

print :
	@echo $(VAR)=$($(VAR))
