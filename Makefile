CC ?= mpicc
CFLAGS ?= -O2
CPPFLAGS ?=
GSDIR ?=
LDFLAGS ?=
DEBUG ?= 1
SHARED ?= 0

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

lib: $(OBJS)
	$(link.o) libexa.$(EXT) $(OBJS) $(LIBS)

print :
	@echo $(VAR)=$($(VAR))
