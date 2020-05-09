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

# Build with MPI
MPI ?= 1

# Install prefix
PREFIX ?= $(HOME)/local/exa

# Dependency locations
GS_DIR ?=

OCCA ?= 1
OCCA_DIR ?= $(HOME)/local/occa

# Fortran interface (TODO: Should be set after a check)
UNDERSCORE ?= 1

### Meta info about the package ###
SRCDIR       =src
BUILDDIR     =build
EXAMPLESDIR  =examples
TESTSDIR     =tests
INTERFACESDIR=interfaces

LDFLAGS  = -L$(GS_DIR)/lib -lgs -lm
INCFLAGS = -I$(GS_DIR)/include -I$(SRCDIR) -I$(INTERFACESDIR)

### Backends ###
# OCCA backend (third party)
occa.dir       = backends/occa
occa.src       = $(wildcard $(occa.dir)/*.c)
occa.obj       = $(patsubst $(occa.dir)/%.c,\
  $(BUILDDIR)/$(occa.dir)/%.o,$(occa.src))
occa.incflags += -I$(occa.dir) -I$(OCCA_DIR)/include

ifneq ($(OCCA),0)
  LDFLAGS += -L$(OCCA_DIR)/lib -locca
  obj = $(occa.obj)
endif

$(BUILDDIR)/$(occa.dir)/%.o: $(occa.dir)/%.c
	$(compile.c) $(occa.incflags) -c $< -o $@

# native backend
native.dir       = backends/native
native.src       = $(wildcard $(native.dir)/*.c)
native.incflags += -I$(native.dir)
native.obj       = $(patsubst $(native.dir)/%.c,\
  $(BUILDDIR)/$(native.dir)/%.c.o,$(native.src))
obj             += $(native.obj)

$(BUILDDIR)/$(native.dir)/%.c.o: $(native.dir)/%.c
	$(compile.c) $(native.incflags) -c $< -o $@

### Include template makefile ###
-include exa-base.mk

### Make targets ###
.PHONY: all
all: lib examples tests install

.PHONY: lib
lib: lib-base

.PHONY: install
install: install-base lib
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
