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
            int offsite = 2;
            if (level != 0) {
                offsite += level * 2;

                char* resized_pointer = realloc(formated_content, content_length + 1 + 1); // content_length + '\0' + '\n'(back) + '\n'
                formated_content = resized_pointer;
                memmove(formated_content + i + 1, formated_content + i, content_length + 1 - i);
                formated_content[i] = ' ';
                i++;
                content_length++;
                
                resized_pointer = realloc(formated_content, content_length + 1 + 1 + offsite); // content_length + '\0' + '\n'
                formated_content = resized_pointer;
                memmove(formated_content + i + 1 + 1 + offsite, formated_content + i + 1, content_length - i);
                
                formated_content[i+1] = '\n';
                for (int offsite_count = 0; offsite_count < offsite; offsite_count++) {
                    formated_content[i + 2 + offsite_count] = ' ';
                }

                content_length += 3 + offsite;
                i += 3 + offsite;
                level++;

            } else {
                char* resized_pointer = realloc(formated_content, content_length + 1 + 1 + offsite); // content_length + '\0' + '\n'
                formated_content = resized_pointer;
                memmove(formated_content + i + 1 + 1 + offsite, formated_content + i + 1, content_length - i);
                
                formated_content[i+1] = '\n';
                for (int offsite_count = 2; offsite_count <= offsite+1; offsite_count++) {
                    formated_content[i + offsite_count] = ' ';
                }

                content_length += 3 + offsite;
                i += 3 + offsite;
                level++;
            }

            
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