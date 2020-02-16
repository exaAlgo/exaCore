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
INCFLAGS ?=

# Build options
DEBUG ?= 1

# Install prefix
PREFIX ?= $(HOME)/local/exa

# Dependency locations
GS_DIR ?=

OCCA ?= 1
OCCA_DIR ?= $(HOME)/local/occa

### Fortran interface ###
UNDERSCORE ?= 1

### Meta info about the package ###
SRCDIR       =src
BUILDDIR     =build
EXAMPLESDIR  =examples
TESTSDIR     =tests
INTERFACESDIR=interfaces

### Include template makefile ###
-include exa-base.mk

### Make targets ###
.PHONY: all
all: lib examples tests install

.PHONY: lib
lib: lib-base

.PHONY: install
install: install-base lib
	@cp -u $(GS_DIR)/include/*.h $(PREFIX)/include/
	@cp -u interfaces/*.h $(PREFIX)/include/
	@cp -u tests/*.h $(PREFIX)/include/
	@mkdir -p $(PREFIX)/share
	@cp -u exa-base.mk tests/run-tests.sh $(PREFIX)/share

.PHONY: examples
examples: examples-base install

.PHONY: tests
tests: tests-base install
	@cp $(TESTSDIR)/run-tests.sh $(BUILDDIR)/$(TESTSDIR)
	@cd $(BUILDDIR)/$(TESTSDIR) && ./run-tests.sh
