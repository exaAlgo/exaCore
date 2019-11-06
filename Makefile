### Compilers and flags ###
CC ?= mpicc
CFLAGS ?= -O2
CPP ?= cpp
CPPFLAGS ?=
LDFLAGS ?=

### Dependency locations ###
GSDIR ?=

## Build options ###
DEBUG ?= 1
SHARED ?= 0

# OpenCL options
OPENCL ?= 1
OPENCL_INCDIR ?= /usr/include
OPENCL_LIBDIR ?= /usr/lib/x86_64-linux-gnu

# Install prefix
PREFIX ?= $(HOME)/local/exa

### Meta info about the package ###
SRCDIR ?= src
BUILDDIR ?= build
DEPDIR ?= .deps

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))
DEPS = $(patsubst $(BUILDDIR)/%.o,$(DEPDIR)/%.d,$(OBJS))

### Backends ###
ifneq ($(OPENCL),0)
  OpenCL.srcdir   = backends/opencl
  OpenCL.builddir = $(BUILDDIR)/backends/opencl
  OpenCL.c        = $(wildcard $(OpenCL.srcdir)/*.c)
  OpenCL.o        = $(patsubst $(OpenCL.srcdir)/%.c,$(BUILDDIR)/backends/opencl/%.o,$(OpenCL.c))
  OpenCL.cflags  ?= -I$(OpenCL.srcdir) -I$(OPENCL_INCDIR)
  OpenCL.ldflags ?= -L$(OPENCL_LIBDIR) -lOpenCL
  OBJS            += $(OpenCL.o)
endif

### Set various flags ###
INCFLAGS  = -I$(SRCDIR) -I$(GSDIR)/include
compile.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(INCFLAGS) -c
link.o    = $(AR) crs
LIBS      = $(GSDIR)/lib/libgs.a
EXT       = a

ifneq ($(DEBUG),0)
  CFLAGS += -g
endif

ifneq ($(SHARED),0)
  CFLAGS  += -fPIC
  link.o   = $(CC) $(LDFLAGS) -shared -o
  LDFLAGS += -Wl,-whole-archive -L$(GSDIR)/lib -lgs -Wl,-no-whole-archive -lm
  EXT      = so
endif

LIBNAME = libexa.$(EXT)

### Make targets ###
.PHONY: lib
lib: $(OBJS)
	$(link.o) $(BUILDDIR)/$(LIBNAME) $(OBJS) $(LIBS) $(LDFLAGS)

.PHONY: install
install: lib
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp -u $(SRCDIR)/*.h $(GSDIR)/include/*.h $(DESTDIR)$(PREFIX)/include/
	@cp -u $(BUILDDIR)/$(LIBNAME) $(DESTDIR)$(PREFIX)/lib/

$(DEPDIR)/%.d: $(SRCDIR)/%.c
	@$(CPP) $(CFLAGS) $(INCFLAGS) $< -MM -MT $(@:$(DEPDIR)/%.d=$(BUILDDIR)/%.deps) >$@

-include $(DEPS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(BUILDDIR)/%.deps
	$(compile.c) $< -o $@

$(BUILDDIR)/backends/opencl/%.o: $(OpenCL.srcdir)/%.c
	$(compile.c) $(OpenCL.cflags) $< -o $@ $(OpenCL.ldflags)

.PHONY: clean
clean:
	@rm -rf $(BUILDDIR) $(DEPDIR)

.PHONY: print
print :
	@echo $(VAR)=$($(VAR))

$(shell mkdir -p $(BUILDDIR))
$(shell mkdir -p $(OpenCL.builddir))
$(shell mkdir -p $(DEPDIR))
