CFLAGS=-Wall -Wextra -fsanitize=address -fsanitize=leak -g -ggdb

TEST_SRC=$(wildcard *.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))
T=$(patsubst %.c,%_test,$(TEST_SRC))

${TESTS}: ${TEST_SRC} ../gh.h
	gcc ${CFLAGS} -o $@ $(addsuffix .c,$@)

${T}: ${TESTS}
	@./$(patsubst %_test,%,$@) && echo -e "[TEST] $@: \033[32mOK\033[0m" || echo -e "[TEST] $@: \033[31mFAILED\033[0m"

clean:
	rm -f ${TESTS}
test: ${T}

default: test
.PHONY: ${T} test clean
