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

### Update variables ###
LDFLAGS += -L$(GSDIR)/lib -lgs -lm
libname  = exa

### Backends ###
OpenCL.dir       = backends/opencl
OpenCL.src       = $(wildcard $(OpenCL.dir)/*.c)
OpenCL.obj       = $(patsubst $(OpenCL.dir)/%.c,$(BUILDDIR)/$(OpenCL.dir)/%.o,$(OpenCL.src))
OpenCL.incflags += -I$(OpenCL.dir) -I$(OPENCL_INCDIR)
ifneq ($(OPENCL),0)
  LDFLAGS += -L$(OPENCL_LIBDIR) -lOpenCL
  obj     += $(OpenCL.obj)
endif

build/$(OpenCL.dir)/%.o: $(OpenCL.dir)/%.c
	$(compile.c) $(OpenCL.incflags) -c $< -o $@

### Include template makefile ###
-include Makefile.in

.PHONY: install
install: lib
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp -u $(SRCDIR)/*.h $(GSDIR)/include/*.h $(DESTDIR)$(PREFIX)/include/
	@cp -u $(BUILDDIR)/$(prefix)$(libname).$(ext) $(DESTDIR)$(PREFIX)/lib/
