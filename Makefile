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
-include exa-base.mk

.PHONY: lib
lib: lib-base

.PHONY: examples
examples: examples-base

.PHONY: tests
tests: tests-base

.PHONY: install
install: install-base
	@cp -u $(GSDIR)/include/*.h $(DESTDIR)$(PREFIX)/include/
	@mkdir -p $(DESTDIR)$(PREFIX)/share
	@cp -u exa-base.mk $(DESTDIR)$(PREFIX)/share

.PHONY: all
all: lib examples tests install
