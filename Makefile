RM := rm -rf

SRC := $(wildcard ./src/*.c)

CC ?= gcc

C_FLAGS := -Wall -Wextra -g -std=c99

ifeq ($(CC),clang)
	C_FLAGS += -Weverything -Wno-disabled-macro-expansion
endif

all: main.out

func_test: functionality.out


main.out:
	$(CC) $(C_FLAGS) -Iinc ./app/main.c $(SRC) -o "cpu_tracker.out" -pthread

functionality.out:
	$(CC) $(C_FLAGS) -Iinc ./test/functionality.c $(SRC) -o "func_test.out" -pthread

clean:
	$(RM) main.out