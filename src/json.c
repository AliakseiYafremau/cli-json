#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* simplify_json(const char json_content[]) {
    size_t length = strlen(json_content);

    char* simplified_content = (char*)malloc(length + 1);
    if (!simplified_content) return NULL;

    const char* source = json_content;
    char* destination = simplified_content;

    bool is_string = false;

    while (*source) {
        if (is_string) {
            *destination = *source;
            destination++;
            if (*source == '"') {
                is_string = false;
            }
        } else if (*source != ' ' && *source != '\n' && *source != '\t' && *source != '\r') {
            *destination = *source;
            destination++;
            if (*source == '"') {
                is_string = true;
            }
        }
        source++;
    }

    *destination = '\0';

    return simplified_content;
}