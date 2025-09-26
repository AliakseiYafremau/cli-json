[private]
default:
    just --list

@build:
    gcc -Wall -Iinclude src/main.c src/file_management.c src/json.c src/format.c -o program

@run: build
    program format

@clean:
    rm -f test_runner
    rm -f main

@generate-db:
    bear -- just build

@lint: generate-db
    run-clang-tidy -p . -header-filter='.*'

@format:
    find src -type f -name '*.c' -exec clang-format -i {} +

@test:
    gcc -Wall -Iinclude -o test_runner tests/check_json_simplify.c src/file_management.c src/json.c src/format.c -lcheck
    ./test_runner
