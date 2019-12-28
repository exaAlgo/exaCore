### User configurations ###
# Compilers and flags
CC ?= mpicc
CXX ?= mpic++
FC ?= mpifort
CFLAGS ?= -O0
CXXFLAGS ?= -O0
FFLAGS ?= -O0
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
# OCCA Backend
OCCA ?= 1
OCCA_DIR ?= $(HOME)/local/occa

### Fortran interface ###
UNDERSCORE ?= 1

### Meta info about the package ###
SRCDIR       = src
BUILDDIR     = build
EXAMPLESDIR  = examples
TESTSDIR     = tests
INTERFACESDIR=interfaces

LDFLAGS += -L$(GSDIR)/lib -lgs -lm
incflags = -I$(GSDIR)/include
libname  = exa
obj      =

### Backends ###
# Backend = codegen + tuning + dispatch
# OCCA backend (third party)
occa.dir       = backends/occa
occa.src       = $(wildcard $(occa.dir)/*.c)
occa.obj       = $(patsubst $(occa.dir)/%.c,\
  $(BUILDDIR)/$(occa.dir)/%.o,$(occa.src))
occa.incflags += -I$(occa.dir) -I$(OCCA_DIR)/include
ifneq ($(OCCA),0)
  LDFLAGS += -L$(OCCA_DIR)/lib -locca
  obj     += $(occa.obj)
endif

$(BUILDDIR)/$(occa.dir)/%.o: $(occa.dir)/%.c
	$(compile.c) $(occa.incflags) -c $< -o $@

### Include template makefile ###
-include exa-base.mk

### Make targets ###
.PHONY: all
all: interfaces lib examples tests install

.PHONY: interfaces
interfaces: interfaces-base

.PHONY: lib
lib: lib-base interfaces

.PHONY: install
install: install-base lib
	@cp -u $(GSDIR)/include/*.h $(DESTDIR)$(PREFIX)/include/
	@cp -u interfaces/*.h $(DESTDIR)$(PREFIX)/include/
	@mkdir -p $(DESTDIR)$(PREFIX)/share
	@cp -u exa-base.mk tests/run-tests.sh $(DESTDIR)$(PREFIX)/share

.PHONY: examples
examples: examples-base install

.PHONY: tests
tests: tests-base install
	@cp $(TESTSDIR)/t[0-9][0-9]-*.[^cf]* $(BUILDDIR)/$(TESTSDIR)/
	@cp $(TESTSDIR)/run-tests.sh $(BUILDDIR)/$(TESTSDIR)
	@cd $(BUILDDIR)/$(TESTSDIR) && ./run-tests.sh
