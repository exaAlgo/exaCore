## User configurations ###
# Compilers and flags
CC ?= mpicc
CFLAGS ?= -O0
CPP ?= cpp
CPPFLAGS ?=
LDFLAGS ?=

# Build options
DEBUG ?= 1

# Install prefix
PREFIX ?= $(HOME)/local/exa

# Dependency locations
GSDIR ?=

# OpenCL options
OPENCL ?= 1
OPENCL_INCDIR ?= /usr/include
OPENCL_LIBDIR ?= /usr/lib/x86_64-linux-gnu

### Meta info about the package ###
SRCDIR      = src
BUILDDIR    = build
DEPDIR      = .deps
EXAMPLESDIR = examples
TESTSDIR    = tests

LDFLAGS += -L$(GSDIR)/lib -lgs -lm
incflags = -I$(GSDIR)/include
libname  = exa
obj      =

### Backends ###
OpenCL.dir       = backends/opencl
OpenCL.src       = $(wildcard $(OpenCL.dir)/*.c)
OpenCL.obj       = $(patsubst $(OpenCL.dir)/%.c,$(BUILDDIR)/$(OpenCL.dir)/%.o,$(OpenCL.src))
OpenCL.incflags += -I$(OpenCL.dir) -I$(OPENCL_INCDIR)
ifneq ($(OPENCL),0)
  LDFLAGS += -L$(OPENCL_LIBDIR) -lOpenCL
  obj     += $(OpenCL.obj)
endif

$(BUILDDIR)/$(OpenCL.dir)/%.o: $(OpenCL.dir)/%.c
	$(compile.c) $(OpenCL.incflags) -c $< -o $@

### Include template makefile ###
-include Makefile.in

.PHONY: install
install: lib
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp -u $(SRCDIR)/*.h $(GSDIR)/include/*.h $(DESTDIR)$(PREFIX)/include/
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@cp -u $(BUILDDIR)/$(prefix)$(libname).$(ext) $(DESTDIR)$(PREFIX)/lib/
	@mkdir -p $(DESTDIR)$(PREFIX)/share
	@cp -u Makefile.in $(DESTDIR)$(PREFIX)/share
