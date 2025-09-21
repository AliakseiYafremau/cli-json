#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* format_json(char* content_pointer) {
    simplify_json(content_pointer);

    int content_length = strlen(content_pointer);
    char* formated_content = malloc(content_length + 1);
    strcpy(formated_content, content_pointer);

    int level = 0;

    for (int i = 0; i < content_length; i++) {
        if (formated_content[i] == '{') {
            char* resized_pointer = realloc(formated_content, content_length + 1 + 3);
            formated_content = resized_pointer;
            memmove(formated_content + i + 3 + 1, formated_content + i + 1, content_length - i);
            formated_content[i+1] = '\n';
            formated_content[i+2] = ' ';
            formated_content[i+3] = ' ';

            content_length += 3;
            i += 3;
            level++;
        }
        if (formated_content[i] == '}') {
            char* resized_pointer = realloc(formated_content, content_length + 1 + 3);
            formated_content = resized_pointer;
            memmove(formated_content + i + 1, formated_content + i, content_length - i); // [1, 2, 3, 0]
            formated_content[i] = '\n';

            content_length += 1;
            i += 1;
            level--;
        }
    }

    return formated_content;
}