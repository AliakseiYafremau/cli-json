#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Get the file size
long file_size(FILE* file) {
    if (!file) {
        return -1;
    }
    if (fseek(file, 0, SEEK_END) != 0) {
        return -1;
    }
    long file_length = ftell(file);
    if (file_length < 0) {
        return -1;
    }
    errno = 0;
    rewind(file);
    if (errno != 0) {
        return -1;
    }
    return file_length;
}

// Return the file content
char* read_file(const char file_name[]) {
    FILE* file = fopen(file_name, "r");
    if (!file) {
        return NULL;
    }

    long length = file_size(file);
    if (length < 0) {
        fclose(file);
        return NULL;
    }

    char* file_content = malloc(length + 1);
    if (!file_content) {
        fclose(file);
        return NULL;
    }

    size_t read_bytes = fread(file_content, 1, length, file);
    if (read_bytes != (size_t)length) {
        free(file_content);
        fclose(file);
        errno = EIO;
        return NULL;
    }
    file_content[length] = '\0';

    fclose(file);

    return file_content;
}