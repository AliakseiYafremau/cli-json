[private]
default:
    just run

@build:
    gcc -Iinclude src/main.c src/file_management.c src/json.c src/format.c -o program

@run: build
    program format

@clean:
    rm -f main