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
# Exa backend based on loopy
LOOPY ?= 0

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
# 1. loopy backend with CUDA and OpenCL dispatch
loopy.dir       = backends/loopy
loopy.src       = $(wildcard $(loopy.dir)/*.c)
loopy.obj       = $(patsubst $(loopy.dir)/%.c,$(BUILDDIR)/$(loopy.dir)/%.o,$(loopy.src))
loopy.incflags += -I$(loopy.dir)

ifneq ($(LOOPY),0)
  obj     += $(loopy.obj)
endif

$(BUILDDIR)/$(loopy.dir)/%.o: $(loopy.dir)/%.c
	$(compile.c) $(loopy.incflags) -c $< -o $@

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
