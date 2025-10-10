[private]
default:
    just --list

@build:
    gcc -Wall -Iinclude src/main.c src/file_management.c src/json.c src/format.c -o cli-json

@clean:
    rm -f test_runner
    rm -f cli-json

@generate-db:
    bear -- just build

@lint: generate-db
    run-clang-tidy -p . -header-filter='.*'

@format:
    find src -type f -name '*.c' -exec clang-format -i {} +
    find tests -type f -name '*.c' -exec clang-format -i {} +

@test:
    gcc -Wall -Iinclude -o test_runner tests/main.c tests/check_simplify_json.c tests/check_format_json.c src/file_management.c src/json.c src/format.c -lcheck
    ./test_runner
