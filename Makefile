CC ?= mpicc
CFLAGS ?= -O2
CPPFLAGS ?=
LDFLAGS ?=
DEBUG ?= 1
DEPDIR ?= .deps

deps.o = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d
compile.c = $(CC) $(deps.o) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
link.o = $(CC) $(LDFLAGS) $(TARGET_ARCH)

%.o : %.c
%.o : %.c $(DEPDIR)/%.d | $(DEPDIR)
    $(compile.c) -o $<

$(DEPDIR): ; @mkdir -p $@

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
DEPFILES = $(SRCS:%.c=$(DEPDIR)/%.d)
$(DEPFILES):

include $(wildcard $(DEPFILES))
