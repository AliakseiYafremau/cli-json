#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* format_json(char* content_pointer) {
    simplify_json(content_pointer);
    int content_length = strlen(content_pointer);

    printf("%d", content_length);
    for (int i = 0; i < content_length; i++) {
        if (!(strcmp(content_pointer[i], '{'))) {
            char* new_pointer = realloc(content_pointer, content_length + 4);
            memmove(new_pointer + i + 3, new_pointer + i, content_length + 1 - i);
        }
    }
}