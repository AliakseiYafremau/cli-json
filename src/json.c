#include <stdlib.h>
#include <string.h>

char* simplify_json(const char json_content[]) {
    size_t length = strlen(json_content);

    char* simplified_content = (char*)malloc(length + 1);
    if (!simplified_content) return NULL;

    const char* source = json_content;
    char* destination = simplified_content;

    while (*source) {
        if (*source != ' ' && *source != '\n') {
            *destination = *source;
            destination++;
        }
        source++;
    }

    *destination = '\0';

    return simplified_content;
}