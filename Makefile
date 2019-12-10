### User configurations ###
# Compilers and flags
CC ?= mpicc
CXX ?= mpic++
CFLAGS ?= -O0
CXXFLAGS ?= -O0
CPP ?= cpp
CPPFLAGS ?=
LDFLAGS ?=

# Build options
DEBUG ?= 1

# Install prefix
PREFIX ?= $(HOME)/local/exa

# Dependency locations
GSDIR ?=

### Backends ###
# exa native backend based on loopy and OpenCl
NATIVE ?= 1
OPENCL_INCDIR ?= /usr/include
OPENCL_LIBDIR ?= /usr/lib/x86_64-linux-gnu

# OCCA Backend
OCCA ?= 1
OCCA_DIR ?= $(HOME)/local/occa

### Meta info about the package ###
SRCDIR      = src
BUILDDIR    = build
EXAMPLESDIR = examples
TESTSDIR    = tests

LDFLAGS += -L$(GSDIR)/lib -lgs -lm
incflags = -I$(GSDIR)/include
libname  = exa
obj      =

### Backends ###
# Backend = codegen + tuning + dispatch
# TODO:
# 1. exa backend based on loopy + opencl
native.dir       = backends/native
native.src       = $(wildcard $(native.dir)/*.c)
native.obj       = $(patsubst $(native.dir)/%.c,$(BUILDDIR)/$(native.dir)/%.o,$(native.src))
native.incflags += -I$(native.dir) -I$(OPENCL_INCDIR)

ifneq ($(NATIVE),0)
  LDFLAGS += -L$(OPENCL_LIBDIR) -lOpenCL
  obj     += $(native.obj)
endif

$(BUILDDIR)/$(native.dir)/%.o: $(native.dir)/%.c
	$(compile.c) $(native.incflags) -c $< -o $@

# 2. occa backend (third party)
occa.dir       = backends/occa
occa.src       = $(wildcard $(occa.dir)/*.c)
occa.obj       = $(patsubst $(occa.dir)/%.c,$(BUILDDIR)/$(occa.dir)/%.o,$(occa.src))
occa.incflags += -I$(occa.dir) -I$(OCCA_DIR)/include
ifneq ($(OCCA),0)
  LDFLAGS += -L$(OCCA_DIR)/lib -locca
  obj     += $(occa.obj)
endif

$(BUILDDIR)/$(occa.dir)/%.o: $(occa.dir)/%.c
	$(compile.c) $(occa.incflags) -c $< -o $@

### Include template makefile ###
-include exa-base.mk

.PHONY: lib
lib: lib-base

.PHONY: examples
examples: examples-base

.PHONY: tests
tests: tests-base
	@cp $(TESTSDIR)/t[0-9][0-9]-*.[^c]* $(BUILDDIR)/$(TESTSDIR)/
	@cp $(TESTSDIR)/run-tests.sh $(BUILDDIR)/$(TESTSDIR)
	@cd $(BUILDDIR)/$(TESTSDIR) && ./run-tests.sh

.PHONY: install
install: install-base
	@cp -u $(GSDIR)/include/*.h $(DESTDIR)$(PREFIX)/include/
	@mkdir -p $(DESTDIR)$(PREFIX)/share
	@cp -u exa-base.mk tests/run-tests.sh $(DESTDIR)$(PREFIX)/share

.PHONY: all
all: lib examples tests install
