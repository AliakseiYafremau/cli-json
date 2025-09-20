#include <stdio.h>
#include <stdlib.h>


// Get the file size
long file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long file_length = ftell(file);
    rewind(file);
    return file_length;
}

// Return the file content
char* read_file(const char file_name[]) {
    FILE *file = fopen(file_name, "r");

    long length = file_size(file);
    char* file_content = malloc(length + 1);
    fread(file_content, 1, length, file);
    file_content[length] = '\0';

    fclose(file);

    return file_content;
}