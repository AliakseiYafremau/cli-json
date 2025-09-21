#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "format.h"
#include "file_management.h"


int parse_arguments(int argc, char *argv[]);


int main(int argc, char *argv[])
{
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    int result = parse_arguments(argc, argv);
    
    return result;
}


int parse_arguments(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Параметры не введены\n");
        return 1;
    } else if (!(strcmp(argv[1], "read"))) {
        char* file_content_pointer = read_file("example.json");
        printf("%s\n", file_content_pointer);
        free(file_content_pointer);
    } else if (!(strcmp(argv[1], "format"))) {
        char* file_content_pointer = read_file("example.json");
        char* formated = format_json(file_content_pointer);
        printf("%s\n", formated);
        free(file_content_pointer);
        free(formated);
    }
    
    return 0;
}