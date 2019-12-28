### Set source files and objects ###
src.c = $(wildcard $(SRCDIR)/*.c)
obj  += $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.c.o,$(src.c))

interfaces.c   = $(wildcard $(INTERFACESDIR)/*.c)
interfaces.obj = $(patsubst $(INTERFACESDIR)/%.c,\
  $(BUILDDIR)/$(INTERFACESDIR)/%.c.o,$(interfaces.c))
obj           += $(interfaces.obj)

native.dir       = backends/native
native.src       = $(wildcard $(native.dir)/*.c)
native.incflags += -I$(native.dir)
native.obj       = $(patsubst $(native.dir)/%.c,\
  $(BUILDDIR)/$(native.dir)/%.c.o,$(native.src))
obj             += $(native.obj)

example.src     = $(wildcard $(EXAMPLESDIR)/*.c)
example.obj     = $(patsubst $(EXAMPLESDIR)/%.c,\
  $(BUILDDIR)/$(EXAMPLESDIR)/%,$(example.src))
example.ldflags = -L$(BUILDDIR) -l$(libname) $(LDFLAGS)

test.c   = $(wildcard $(TESTSDIR)/*.c)
test.f   = $(wildcard $(TESTSDIR)/*.f)
test.obj = $(patsubst $(TESTSDIR)/%.c,\
  $(BUILDDIR)/$(TESTSDIR)/%-c,$(test.c))
test.obj+= $(patsubst $(TESTSDIR)/%.f,\
  $(BUILDDIR)/$(TESTSDIR)/%-f,$(test.f))

CMMNFLAGS ?=

### Set various flags ###
ifneq ($(UNDERSCORE),0)
  CMMNFLAGS += -DEXA_UNDERSCORE
endif

ifneq ($(DEBUG),0)
  CMMNFLAGS += -g -DEXA_DEBUG
endif

CMMNFLAGS += -fPIC

incflags += -I$(SRCDIR) -I$(INTERFACESDIR)
ext       = so
prefix    = lib

compile.c  =$(CC) $(CFLAGS) $(CMMNFLAGS) $(CPPFLAGS) $(incflags)
compile.cpp=$(CXX) $(CXXFLAGS) $(CMMNFLAGS) $(CPPFLAGS) $(incflags)
compile.f  =$(FC) $(FFLAGS) $(CMMNFLAGS) $(CPPFLAGS) $(incflags)

link.c   = $(CC) -shared -o
link.cpp = $(CXX) -shared -o
link.f   = $(FC) -shared -o

### Make targets ###
.PHONY: all-base
all-base: interfaces-base lib-base install-base examples-base\
  tests-base

.PHONY: interfaces-base
interfaces-base: $(interfaces.obj)

.PHONY: lib-base
lib-base: $(obj) interfaces-base
	$(link.c) $(BUILDDIR)/lib$(libname).$(ext) $(obj) $(LDFLAGS)

.PHONY: install-base
install-base: lib-base
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp -u $(SRCDIR)/*.h $(DESTDIR)$(PREFIX)/include/
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@cp -u $(BUILDDIR)/$(prefix)$(libname).$(ext)\
    $(DESTDIR)$(PREFIX)/lib/

$(BUILDDIR)/%.c.o: $(SRCDIR)/%.c
	$(compile.c) -c $< -o $@

$(BUILDDIR)/$(native.dir)/%.c.o: $(native.dir)/%.c
	$(compile.c) $(native.incflags) -c $< -o $@

$(BUILDDIR)/$(INTERFACESDIR)/%.c.o: $(INTERFACESDIR)/%.c
	$(compile.c) -c $< -o $@

.PHONY: examples-base
examples-base: install-base $(example.obj)

$(BUILDDIR)/examples/%: $(EXAMPLESDIR)/%.c
	$(compile.c) $< -o $@ $(example.ldflags)

.PHONY: tests-base
tests-base: install-base $(test.obj)

$(BUILDDIR)/tests/%-c: $(TESTSDIR)/%.c
	$(compile.c) $< -o $@ $(example.ldflags)

$(BUILDDIR)/tests/%-f: $(TESTSDIR)/%.f
	$(compile.f) $< -o $@ $(example.ldflags)

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
