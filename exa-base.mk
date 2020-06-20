#TODO: Should be based on the platform
libExt    ?=so
libPrefix ?=lib
libName   ?=exa

### Compile src/ ###
src.c   = $(wildcard $(SRCDIR)/*.c)
src.obj = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/src/%.c.o,$(src.c))
obj    += $(src.obj)

$(BUILDDIR)/src/%.c.o: $(SRCDIR)/%.c
	$(compile.c) -c $< -o $@

### Compile interfaces/ ###
interfaces.c   = $(wildcard $(INTERFACESDIR)/*.c)
interfaces.obj = $(patsubst $(INTERFACESDIR)/%.c,\
  $(BUILDDIR)/interfaces/%.c.o,$(interfaces.c))
obj += $(interfaces.obj)

$(BUILDDIR)/interfaces/%.c.o: $(INTERFACESDIR)/%.c
	$(compile.c) -c $< -o $@

### Compile examples/ ###
examples.src = $(wildcard $(EXAMPLESDIR)/*.c)
examples.obj = $(patsubst $(EXAMPLESDIR)/%.c,\
  $(BUILDDIR)/examples/%,$(examples.src))
examples.ldflags = -L$(BUILDDIR) -l$(libName) $(LDFLAGS)

$(BUILDDIR)/examples/%: $(EXAMPLESDIR)/%.c
	$(compile.c) $< -o $@ $(examples.ldflags)

### Compile tests/ ###
tests.c   = $(wildcard $(TESTSDIR)/*.c)
tests.f   = $(wildcard $(TESTSDIR)/*.f)
tests.obj = $(patsubst $(TESTSDIR)/%.c,\
  $(BUILDDIR)/tests/%-c,$(tests.c))
tests.obj+= $(patsubst $(TESTSDIR)/%.f,\
  $(BUILDDIR)/tests/%-f,$(tests.f))
tests.incflags= -I$(TESTSDIR)
tests.ldflags = -L$(BUILDDIR) -l$(libName) $(LDFLAGS)

$(BUILDDIR)/tests/%-c: $(TESTSDIR)/%.c
	$(compile.c) $(tests.incflags) $< -o $@ $(tests.ldflags)

$(BUILDDIR)/tests/%-f: $(TESTSDIR)/%.f
	$(compile.f) $(tests.incflags) $< -o $@ $(tests.ldflags)

EXAFLAGS ?=

### Set various flags ###
ifneq ($(UNDERSCORE),0)
  EXAFLAGS += -DEXA_UNDERSCORE
endif

ifneq ($(DEBUG),0)
  EXAFLAGS += -g -DEXA_DEBUG
endif

ifneq ($(MPI),0)
	EXAFLAGS += -DEXA_MPI
endif

EXAFLAGS += -fPIC

compile.c  =$(CC) $(CFLAGS) $(EXAFLAGS) $(CPPFLAGS) $(INCFLAGS)
compile.cpp=$(CXX) $(CXXFLAGS) $(EXAFLAGS) $(CPPFLAGS) $(INCFLAGS)
compile.f  =$(FC) $(FFLAGS) $(EXAFLAGS) $(CPPFLAGS) $(INCFLAGS)

link.c   = $(CC) -shared -o
link.cpp = $(CXX) -shared -o
link.f   = $(FC) -shared -o

### Make targets ###
.PHONY: all-base
all-base: lib-base install-base examples-base tests-base

.PHONY: lib-base
lib-base: $(obj)
	$(link.c) $(BUILDDIR)/$(libPrefix)$(libName).$(libExt) $(obj) $(LDFLAGS)

.PHONY: install-base
install-base: lib-base
	@mkdir -p $(PREFIX)/include
	@cp -u $(SRCDIR)/*.h $(PREFIX)/include/
	@mkdir -p $(PREFIX)/lib
	@cp -u $(BUILDDIR)/$(libPrefix)$(libName).$(libExt) $(PREFIX)/lib/

.PHONY: examples-base
examples-base: install-base $(examples.obj)

.PHONY: tests-base
tests-base: install-base $(tests.obj)
	@cp $(TESTSDIR)/t[0-9][0-9][0-9]-*.[^cf]* $(BUILDDIR)/$(TESTSDIR)/

.PHONY: clean
clean:
	@rm -rf $(BUILDDIR)

.PHONY: print
print :
	@echo $(VAR)=$($(VAR))

$(shell mkdir -p $(BUILDDIR))
$(shell mkdir -p $(BUILDDIR)/$(EXAMPLESDIR))
$(shell mkdir -p $(BUILDDIR)/$(TESTSDIR))
$(shell mkdir -p $(BUILDDIR)/$(native.dir))
$(shell mkdir -p $(BUILDDIR)/$(occa.dir))
$(shell mkdir -p $(BUILDDIR)/$(INTERFACESDIR))
$(shell mkdir -p $(BUILDDIR)/$(SRCDIR))
