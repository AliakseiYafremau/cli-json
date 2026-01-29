#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_management.h"
#include "format.h"
#include "json.h"

int parse_arguments(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    int result = parse_arguments(argc, argv);
    return result;
}

int parse_arguments(int argc, char* argv[]) {
    if (argc == 1) {
        printf("Parameters not specified. For help, try 'cli-json help'.\n");
        return 1;
    }
    if (!(strcmp(argv[1], "help"))) {
        printf(
            "CLI-json is a tool is designed for fast formatting of JSON "
            "files.\n\n"
            "Available commands:\n"
            "\thelp                     print this message\n"
            "\tread <filename>          output formatted content of the file\n"
            "\tformat <filename>        format the file\n"
            "\tvalidate <filename>      validate json, prints true/false\n");
        return 0;
    }

    if (strcmp(argv[1], "read") && strcmp(argv[1], "format") &&
        strcmp(argv[1], "validate")) {
        printf("Incorrect parameters. For help, try 'cli-json help'.\n");
        return 1;
    }

    char* file_content_pointer = read_file(argv[2]);

    if (!file_content_pointer) {
        printf("Incorrect file path entered: '%s'\n", argv[2]);
        return 1;
    }
    if (!(strcmp(argv[1], "validate"))) {
        int valid = validate_json(file_content_pointer);
        printf("%s\n", valid ? "true" : "false");
        free(file_content_pointer);
        return valid ? 0 : 1;
    }

    char* formated = format_json(file_content_pointer);
    if (!(strcmp(argv[1], "read"))) {
        printf("%s\n", file_content_pointer);
    } else if (!(strcmp(argv[1], "format"))) {
        FILE* file_to_format = fopen(argv[2], "w");
        if (!file_to_format) {
            printf("Не удалось открыть файл для записи: '%s')", argv[2]);
            free(formated);
            free(file_content_pointer);
            return 1;
        }
        fprintf(file_to_format, "%s", formated);
        fclose(file_to_format);
    }

    free(formated);
    free(file_content_pointer);

    return 0;
}
