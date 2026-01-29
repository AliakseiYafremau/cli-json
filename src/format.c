#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

static bool ensure_capacity(char** buffer, size_t* capacity, size_t needed) {
    if (needed < *capacity) return true;
    while (needed >= *capacity) {
        *capacity *= 2;
    }
    char* resized = realloc(*buffer, *capacity);
    if (!resized) return false;
    *buffer = resized;
    return true;
}

static bool append_char(char** buffer, size_t* capacity, size_t* length,
                        char value) {
    if (!ensure_capacity(buffer, capacity, *length + 2)) return false;
    (*buffer)[(*length)++] = value;
    return true;
}

static bool append_indent(char** buffer, size_t* capacity, size_t* length,
                          int level) {
    size_t needed = *length + (size_t)level * 2 + 1;
    if (!ensure_capacity(buffer, capacity, needed)) return false;
    for (int i = 0; i < level * 2; i++) {
        (*buffer)[(*length)++] = ' ';
    }
    return true;
}

static bool object_has_nested_compound(const char* input, size_t start,
                                       size_t input_length) {
    bool in_string = false;
    bool escape = false;
    for (size_t i = start + 1; i < input_length; i++) {
        char c = input[i];
        if (in_string) {
            if (escape) {
                escape = false;
            } else if (c == '\\') {
                escape = true;
            } else if (c == '"') {
                in_string = false;
            }
            continue;
        }
        if (c == '"') {
            in_string = true;
            continue;
        }
        if (c == '{' || c == '[') return true;
        if (c == '}') break;
    }
    return false;
}

char* format_json(char* content_pointer) {
    char* simplified = simplify_json(content_pointer);
    if (!simplified) return NULL;

    size_t input_length = strlen(simplified);
    size_t capacity = input_length * 4 + 16;
    char* formated_content = malloc(capacity);
    if (!formated_content) {
        free(simplified);
        return NULL;
    }

    char* container_stack = malloc(input_length + 1);
    bool* inline_object_stack = malloc((input_length + 1) * sizeof(bool));
    if (!container_stack || !inline_object_stack) {
        free(simplified);
        free(formated_content);
        free(container_stack);
        free(inline_object_stack);
        return NULL;
    }

    size_t out_len = 0;
    int level = 0;
    int stack_top = -1;
    bool in_string = false;
    bool escape = false;

    for (size_t i = 0; i < input_length; i++) {
        char c = simplified[i];

        if (in_string) {
            if (!append_char(&formated_content, &capacity, &out_len, c)) break;
            if (escape) {
                escape = false;
            } else if (c == '\\') {
                escape = true;
            } else if (c == '"') {
                in_string = false;
            }
            continue;
        }

        if (c == '"') {
            if (!append_char(&formated_content, &capacity, &out_len, c)) break;
            in_string = true;
            continue;
        }

        if (c == '{' && i + 1 < input_length && simplified[i + 1] == '}') {
            if (!append_char(&formated_content, &capacity, &out_len, '{')) break;
            if (!append_char(&formated_content, &capacity, &out_len, '}')) break;
            i++;
            continue;
        }

        if (c == '{') {
            bool inline_object =
                (stack_top == 0 && container_stack[stack_top] == '[' &&
                 !object_has_nested_compound(simplified, i, input_length));
            stack_top++;
            container_stack[stack_top] = '{';
            inline_object_stack[stack_top] = inline_object;
            level++;

            if (!append_char(&formated_content, &capacity, &out_len, '{')) break;
            if (inline_object) continue;
            if (!append_char(&formated_content, &capacity, &out_len, '\n'))
                break;
            if (!append_indent(&formated_content, &capacity, &out_len, level))
                break;
            continue;
        }

        if (c == '[') {
            stack_top++;
            container_stack[stack_top] = '[';
            inline_object_stack[stack_top] = false;
            level++;

            if (!append_char(&formated_content, &capacity, &out_len, '[')) break;
            if (!append_char(&formated_content, &capacity, &out_len, '\n'))
                break;
            if (!append_indent(&formated_content, &capacity, &out_len, level))
                break;
            continue;
        }

        if (c == ']') {
            level--;
            if (!append_char(&formated_content, &capacity, &out_len, '\n'))
                break;
            if (!append_indent(&formated_content, &capacity, &out_len, level))
                break;
            if (!append_char(&formated_content, &capacity, &out_len, ']')) break;
            if (stack_top >= 0 && container_stack[stack_top] == '[') {
                stack_top--;
            }
            continue;
        }

        if (c == '}') {
            bool inline_object =
                (stack_top >= 0 && container_stack[stack_top] == '{' &&
                 inline_object_stack[stack_top]);
            level--;
            if (!inline_object) {
                if (!append_char(&formated_content, &capacity, &out_len, '\n'))
                    break;
                if (!append_indent(&formated_content, &capacity, &out_len,
                                   level))
                    break;
            }
            if (!append_char(&formated_content, &capacity, &out_len, '}')) break;
            if (stack_top >= 0 && container_stack[stack_top] == '{') {
                stack_top--;
            }
            continue;
        }

        if (c == ',') {
            bool inline_object =
                (stack_top >= 0 && container_stack[stack_top] == '{' &&
                 inline_object_stack[stack_top]);
            if (!append_char(&formated_content, &capacity, &out_len, ',')) break;
            if (inline_object) {
                if (!append_char(&formated_content, &capacity, &out_len, ' '))
                    break;
            } else {
                if (!append_char(&formated_content, &capacity, &out_len, '\n'))
                    break;
                if (!append_indent(&formated_content, &capacity, &out_len,
                                   level))
                    break;
            }
            continue;
        }

        if (c == ':') {
            if (!append_char(&formated_content, &capacity, &out_len, ':')) break;
            if (!append_char(&formated_content, &capacity, &out_len, ' ')) break;
            continue;
        }

        if (!append_char(&formated_content, &capacity, &out_len, c)) break;
    }

    formated_content[out_len] = '\0';
    free(simplified);
    free(container_stack);
    free(inline_object_stack);
    return formated_content;
}
