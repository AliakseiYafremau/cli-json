# CLI-JSON

Console tool to format and process JSON files

**Key features**:
- 🔄 formatting (in development)
- ❌ validation (planned)

## Features

```
CLI-json is a tool is designed for fast formatting of JSON files.

    Available commands:
        help                     print this message
        read <filename>         output formatted content of the file
        format <filename>        format the file
```

## Usage

### Requirements

- gcc
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/) (to lint)
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html) (to format)
- [check](https://libcheck.github.io/check/web/install.html) (to test)
- [just](https://just.systems/man/en/) (optional)

### Build

To build the project:
```
gcc -Wall -Iinclude src/main.c src/file_management.c src/json.c src/format.c -o cli-json
```
Or you can use **just**:
```bash
just build
```

To run the project:
```
./cli-json
```
```bash
just run
```

Also you can use linter and formatter ([`clang-tidy`](https://clang.llvm.org/extra/clang-tidy/) and [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html) must be installed)
```bash
just lint
just format
```

To test your code run ([`check`](https://libcheck.github.io/check/web/install.html) must be installed):
```
just test
```