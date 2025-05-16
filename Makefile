CC=gcc
CFLAGS=-Wall -Wextra -fsanitize=address -fsanitize=leak -g -ggdb
BUILDDIR=build

all: test examples

$(BUILDDIR) $(BUILDDIR)/examples $(BUILDDIR)/test &:
	mkdir -p $(BUILDDIR)/examples
	mkdir -p $(BUILDDIR)/test

EXAMPLES=$(patsubst examples/%.c,$(BUILDDIR)/examples/%,$(wildcard examples/*.c))
$(BUILDDIR)/examples/%: examples/%.c gh.h | $(BUILDDIR)/examples
	$(CC) $(CFLAGS) -o $@ $<

TESTS=$(patsubst test/%.c,$(BUILDDIR)/test/%,$(wildcard test/*.c))
$(BUILDDIR)/test/%: test/%.c gh.h | $(BUILDDIR)/test
	$(CC) $(CFLAGS) -o $@ $<

RUNTESTS=$(patsubst $(BUILDDIR)/test/%,run_%,$(TESTS))
run_%: $(BUILDDIR)/test/%
	@$(BUILDDIR)/test/$* && echo -e "[TEST] $*: \033[32mOK\033[0m" || echo -e "[TEST] $*: \033[31mFAILED\033[0m"

test: ${RUNTESTS}
examples: ${EXAMPLES}

.PHONY: clean test examples all
clean:
	rm -rf $(BUILDDIR)
