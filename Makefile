CC ?= mpicc
CFLAGS ?= -O2
CPPFLAGS ?=
GSDIR ?=
LDFLAGS ?=
DEBUG ?= 1
DEPDIR ?= .deps

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
INCFLAGS = -I$(CURDIR)/src -I$(GSDIR)/include

depflags.d = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
compile.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(INCFLAGS) -c
link.o = $(CC) $(LDFLAGS)

%.o : %.c
	$(compile.c) $<

all: $(OBJS)

print :
	@echo $(VAR)=$($(VAR))
