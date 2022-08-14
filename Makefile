CC := gcc
OPTS := -I ./src -Wall -Wextra -Wno-unused-parameter -gdwarf-2
LD_OPTS := -lxcb -lxcb-screensaver
SRCS := $(shell find src/ -name '*.c' -o -name '*.h')
OBJS := $(shell find src/ -name '*.c' | sed 's/src/out/' | sed 's/c$$/o/')
EXES := x-screensaver-query-info
COMPILE_EXE = ${CC} ${OPTS} $^ ${LD_OPTS} -o $@
ENSURE_DIR = mkdir -p $(shell dirname "$@")
.PHONY: all objs clean workspace

all: ${EXES}

tags: ${SRCS}
	ctags $^

compile_flags.txt: Makefile
	for i in ${OPTS}; do printf "%s\n" "$$i"; done > compile_flags.txt

workspace: tags compile_flags.txt

objs: ${OBJS}

x-screensaver-query-info: ${OBJS}
	${COMPILE_EXE}

out/%.o: src/%.c
	${ENSURE_DIR}
	${CC} -c "$<" ${OPTS} -o "$@"

clean:
	-rm tags
	-rm compile_flags.txt
	-rm ${OBJS}
	-rm ${EXES}
