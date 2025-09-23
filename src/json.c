#include <string.h>

// Delete spaces and '\n'
void simplify_json(char json_content[]) {
    char* simplified_content = json_content;
    char* source = json_content;

    while (*source) {
        if ((*source != ' ') && (*source != '\n')) {
            *simplified_content = *source;
            simplified_content++;
        }
        source++;
    }

    *simplified_content = '\0';
}