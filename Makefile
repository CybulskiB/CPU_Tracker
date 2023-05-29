RM := rm -rf

SRC := $(wildcard ./src/*.c)

CC ?= gcc

C_FLAGS := -Wall -Wextra -gdwarf-4 -std=c99

ifeq ($(CC),clang)
	C_FLAGS += -Weverything -Wno-disabled-macro-expansion 
endif

all: main.out

test: test.out


main.out:
	$(CC) $(C_FLAGS) -Iinc ./app/main.c $(SRC) -o "cpu_tracker.out" -pthread

test.out:
	$(CC) $(C_FLAGS) -Iinc ./test/functionality.c $(SRC) -o "func_test.out" -pthread
	$(CC) $(C_FLAGS) -Iinc ./test/watchdog_memory_check.c $(SRC) -o "watchdog_memcheck.out" -pthread
	
clean:
	$(RM) main.out