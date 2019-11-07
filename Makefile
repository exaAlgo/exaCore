## User configurations ###
# Compilers and flags
CC ?= mpicc
CFLAGS ?= -O2
CPP ?= cpp
CPPFLAGS ?=
LDFLAGS ?=

# Dependency locations
GSDIR ?=

# Build options
DEBUG ?= 1

# OpenCL options
OPENCL ?= 1
OPENCL_INCDIR ?= /usr/include
OPENCL_LIBDIR ?= /usr/lib/x86_64-linux-gnu

# Install prefix
PREFIX ?= $(HOME)/local/exa

### Meta info about the package ###
SRCDIR   = src
BUILDDIR = build
DEPDIR   = .deps
EXAMPLESDIR = examples

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))
DEPS = $(patsubst $(BUILDDIR)/%.o,$(DEPDIR)/%.d,$(OBJS))
EXAMPLESRCS = $(wildcard $(EXAMPLESDIR)/*.c)
EXAMPLEOBJS = $(patsubst $(EXAMPLESDIR)/%.c,$(BUILDDIR)/examples/%.o,$(EXAMPLESRCS))

### Set various flags ###
ifneq ($(DEBUG),0)
  CFLAGS += -g
endif

CFLAGS   += -fPIC
incflags  = -I$(SRCDIR) -I$(GSDIR)/include
compile.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(incflags)
link.o    = $(CC) $(LDFLAGS) -shared -o
LDFLAGS  += -L$(GSDIR)/lib -lgs -lm
EXT       = so
LIBNAME   = libexa.$(EXT)

### Backends ###
ifneq ($(OPENCL),0)
  OpenCL.srcdir   = backends/opencl
  OpenCL.builddir = $(BUILDDIR)/backends/opencl
  OpenCL.c        = $(wildcard $(OpenCL.srcdir)/*.c)
  OpenCL.o        = $(patsubst $(OpenCL.srcdir)/%.c,$(BUILDDIR)/backends/opencl/%.o,$(OpenCL.c))
  OpenCL.cflags  ?= -I$(OpenCL.srcdir) -I$(OPENCL_INCDIR)
  OpenCL.ldflags ?= -L$(OPENCL_LIBDIR) -lOpenCL
  LDFLAGS        += $(OpenCL.ldflags)
  OBJS           += $(OpenCL.o)
endif


### Make targets ###
.PHONY: all
all: lib install examples

.PHONY: lib
lib: $(OBJS)
	$(link.o) $(BUILDDIR)/$(LIBNAME) $(OBJS) $(LDFLAGS)

.PHONY: install
install: lib
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp -u $(SRCDIR)/*.h $(GSDIR)/include/*.h $(DESTDIR)$(PREFIX)/include/
	@cp -u $(BUILDDIR)/$(LIBNAME) $(DESTDIR)$(PREFIX)/lib/

$(DEPDIR)/%.d: $(SRCDIR)/%.c
	@$(CPP) $(CFLAGS) $(incflags) $< -MM -MT $(@:$(DEPDIR)/%.d=$(BUILDDIR)/%.deps) >$@

-include $(DEPS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(BUILDDIR)/%.deps
	$(compile.c) -c $< -o $@

$(BUILDDIR)/backends/opencl/%.o: $(OpenCL.srcdir)/%.c
	$(compile.c) $(OpenCL.cflags) -c $< -o $@ $(OpenCL.ldflags)

.PHONY: clean
clean:
	@rm -rf $(BUILDDIR) $(DEPDIR)

.PHONY: examples
examples: $(EXAMPLEOBJS)

$(BUILDDIR)/examples/%.o: $(EXAMPLESDIR)/%.c
	$(compile.c) $< -o $@ -I$(SRCDIR) -I$(OpenCL.srcdir) -L$(BUILDDIR) -lexa

.PHONY: print
print :
	@echo $(VAR)=$($(VAR))

$(shell mkdir -p $(BUILDDIR))
$(shell mkdir -p $(BUILDDIR)/examples)
$(shell mkdir -p $(OpenCL.builddir))
$(shell mkdir -p $(DEPDIR))
