.PHONY: all help build clean generate-db lint format test

CC := gcc
CFLAGS := -Wall -Iinclude

SRC := src/main.c src/file_management.c src/json.c src/format.c
LIB_SRC := src/file_management.c src/json.c src/format.c
TEST_SRC := tests/main.c tests/check_simplify_json.c tests/check_format_json.c tests/check_validate_json.c

all: build

help:
	@echo "Targets:"
	@echo "  build        Build cli-json"
	@echo "  clean        Remove build artifacts"
	@echo "  generate-db  Generate compile_commands.json via bear"
	@echo "  lint         Run clang-tidy (requires generate-db)"
	@echo "  format       Run clang-format on src/ and tests/"
	@echo "  test         Build and run tests"

build:
	$(CC) $(CFLAGS) $(SRC) -o cli-json

clean:
	rm -f test_runner
	rm -f cli-json

generate-db:
	bear -- make build

lint: generate-db
	run-clang-tidy -p . -header-filter='.*'

format:
	find src -type f -name '*.c' -exec clang-format -i {} +
	find tests -type f -name '*.c' -exec clang-format -i {} +

test:
	$(CC) $(CFLAGS) -o test_runner $(TEST_SRC) $(LIB_SRC) -lcheck
	./test_runner
