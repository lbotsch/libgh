CFLAGS=-Wall -Wextra -fsanitize=address -fsanitize=leak -g -ggdb

all: test examples

build:
	mkdir -p build

EXAMPLE_SRC=$(wildcard examples/*.c)
EXAMPLES=$(patsubst examples/%.c,build/%,$(EXAMPLE_SRC))

${EXAMPLES}: ${EXAMPLE_SRC} gh.h build
	gcc ${CFLAGS} -o $@ $(patsubst build/%,examples/%.c,$@)

TEST_SRC=$(wildcard test/*.c)
TESTS=$(patsubst test/%.c,build/%,$(TEST_SRC))
T=$(patsubst test/%.c,%_test,$(TEST_SRC))

${TESTS}: ${TEST_SRC} gh.h build
	gcc ${CFLAGS} -o $@ $(patsubst build/%,test/%.c,$@)

${T}: ${TESTS}
	@./$(patsubst %_test,build/%,$@) && echo -e "[TEST] $@: \033[32mOK\033[0m" || echo -e "[TEST] $@: \033[31mFAILED\033[0m"

test: ${T}
examples: ${EXAMPLES}

clean:
	rm -rf build

.PHONY: ${T} test examples clean
