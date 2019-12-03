src.c = $(wildcard $(SRCDIR)/*.c)
obj  += $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.c.o,$(src.c))

deps = $(patsubst $(BUILDDIR)/%.o,$(DEPDIR)/%.d,$(obj))

example.src     = $(wildcard $(EXAMPLESDIR)/*.c)
example.obj     = $(patsubst $(EXAMPLESDIR)/%.c,$(BUILDDIR)/examples/%,$(example.src))
example.ldflags = -L$(BUILDDIR) -l$(libname) $(LDFLAGS)

test.src        = $(wildcard $(TESTSDIR)/*.c)
test.obj        = $(patsubst $(TESTSDIR)/%.c,$(BUILDDIR)/tests/%,$(test.src))

### Set various flags ###
ifneq ($(DEBUG),0)
  CFLAGS += -g -DEXA_DEBUG
endif
CFLAGS += -fPIC

incflags += -I$(SRCDIR)
ext       = so
prefix    = lib

compile.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(incflags)
link.c    = $(CC) -shared -o

### Make targets ###
.PHONY: all-base
all-base: lib-base install-base examples-base tests-base

.PHONY: lib-base
lib-base: $(obj)
	$(link.c) $(BUILDDIR)/lib$(libname).$(ext) $(obj) $(LDFLAGS)

.PHONY: install-base
install-base: lib-base
	@mkdir -p $(DESTDIR)$(PREFIX)/include
	@cp -u $(SRCDIR)/*.h $(DESTDIR)$(PREFIX)/include/
	@mkdir -p $(DESTDIR)$(PREFIX)/lib
	@cp -u $(BUILDDIR)/$(prefix)$(libname).$(ext) $(DESTDIR)$(PREFIX)/lib/

$(DEPDIR)/%.d: $(SRCDIR)/%.c
	@$(CPP) $(CFLAGS) $(incflags) $< -MM -MT $(@:$(DEPDIR)/%.d=$(BUILDDIR)/%.deps) >$@

-include $(deps)

$(BUILDDIR)/%.c.o: $(SRCDIR)/%.c
	$(compile.c) -c $< -o $@

.PHONY: examples-base
examples-base: lib $(example.obj)

$(BUILDDIR)/examples/%: $(EXAMPLESDIR)/%.c
	$(compile.c) $< -o $@ $(example.ldflags)

.PHONY: tests-base
tests-base: lib $(test.obj)

$(BUILDDIR)/tests/%: $(TESTSDIR)/%.c
	$(compile.c) $< -o $@ $(example.ldflags)

.PHONY: clean
clean:
	@rm -rf $(BUILDDIR) $(DEPDIR)

.PHONY: print
print :
	@echo $(VAR)=$($(VAR))

$(shell mkdir -p $(DEPDIR))
$(shell mkdir -p $(BUILDDIR))
$(shell mkdir -p $(BUILDDIR)/examples)
$(shell mkdir -p $(BUILDDIR)/tests)
$(shell mkdir -p $(BUILDDIR)/$(OpenCL.dir))
