#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"

char* format_json(char* content_pointer) {
    char* simplified = simplify_json(content_pointer);
    if (!simplified) return NULL;

    size_t input_length = strlen(simplified);
    size_t capacity = input_length * 4 + 16;
    char* formated_content = malloc(capacity);
    if (!formated_content) return NULL;

    size_t out_len = 0;
    int level = 0;
    bool in_string = false;
    bool escape = false;
    char* container_stack = malloc(input_length + 1);
    bool* inline_object_stack = malloc((input_length + 1) * sizeof(bool));
    int stack_top = -1;

    for (size_t i = 0; i < input_length; i++) {
        char c = simplified[i];

        if (in_string) {
            if (out_len + 2 >= capacity) {
                capacity *= 2;
                formated_content = realloc(formated_content, capacity);
            }
            formated_content[out_len++] = c;
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
            if (out_len + 2 >= capacity) {
                capacity *= 2;
                formated_content = realloc(formated_content, capacity);
            }
            formated_content[out_len++] = c;
            in_string = true;
            continue;
        }

        if (c == '{' && i + 1 < input_length && simplified[i + 1] == '}') {
            if (out_len + 3 >= capacity) {
                capacity *= 2;
                formated_content = realloc(formated_content, capacity);
            }
            formated_content[out_len++] = '{';
            formated_content[out_len++] = '}';
            i++;
            continue;
        }

        if (c == '{') {
            bool has_nested = false;
            bool scan_in_string = false;
            bool scan_escape = false;
            for (size_t j = i + 1; j < input_length; j++) {
                char sc = simplified[j];
                if (scan_in_string) {
                    if (scan_escape) {
                        scan_escape = false;
                    } else if (sc == '\\') {
                        scan_escape = true;
                    } else if (sc == '"') {
                        scan_in_string = false;
                    }
                    continue;
                }
                if (sc == '"') {
                    scan_in_string = true;
                    continue;
                }
                if (sc == '{' || sc == '[') {
                    has_nested = true;
                    break;
                }
                if (sc == '}') break;
            }

            bool inline_object =
                (stack_top == 0 && container_stack[stack_top] == '[' &&
                 !has_nested);
            stack_top++;
            container_stack[stack_top] = '{';
            inline_object_stack[stack_top] = inline_object;
            level++;
            if (inline_object) {
                if (out_len + 2 >= capacity) {
                    capacity *= 2;
                    formated_content = realloc(formated_content, capacity);
                }
                formated_content[out_len++] = '{';
                continue;
            }

            size_t needed = 2 + (size_t)level * 2;
            while (out_len + needed + 1 >= capacity) capacity *= 2;
            formated_content = realloc(formated_content, capacity);
            formated_content[out_len++] = '{';
            formated_content[out_len++] = '\n';
            for (int s = 0; s < level * 2; s++) {
                formated_content[out_len++] = ' ';
            }
            continue;
        }

        if (c == '[') {
            stack_top++;
            container_stack[stack_top] = '[';
            inline_object_stack[stack_top] = false;
            level++;
            size_t needed = 2 + (size_t)level * 2;
            while (out_len + needed + 1 >= capacity) capacity *= 2;
            formated_content = realloc(formated_content, capacity);
            formated_content[out_len++] = '[';
            formated_content[out_len++] = '\n';
            for (int s = 0; s < level * 2; s++) {
                formated_content[out_len++] = ' ';
            }
            continue;
        }

        if (c == ']') {
            size_t needed = 2 + (size_t)(level - 1) * 2;
            while (out_len + needed + 1 >= capacity) capacity *= 2;
            formated_content = realloc(formated_content, capacity);
            formated_content[out_len++] = '\n';
            for (int s = 0; s < (level - 1) * 2; s++) {
                formated_content[out_len++] = ' ';
            }
            formated_content[out_len++] = ']';
            level--;
            if (stack_top >= 0 && container_stack[stack_top] == '[') {
                stack_top--;
            }
            continue;
        }

        if (c == '}') {
            bool inline_object =
                (stack_top >= 0 && container_stack[stack_top] == '{' &&
                 inline_object_stack[stack_top]);
            if (inline_object) {
                if (out_len + 2 >= capacity) {
                    capacity *= 2;
                    formated_content = realloc(formated_content, capacity);
                }
                formated_content[out_len++] = '}';
            } else {
                size_t needed = 2 + (size_t)(level - 1) * 2;
                while (out_len + needed + 1 >= capacity) capacity *= 2;
                formated_content = realloc(formated_content, capacity);
                formated_content[out_len++] = '\n';
                for (int s = 0; s < (level - 1) * 2; s++) {
                    formated_content[out_len++] = ' ';
                }
                formated_content[out_len++] = '}';
            }
            level--;
            if (stack_top >= 0 && container_stack[stack_top] == '{') {
                stack_top--;
            }
            continue;
        }

        if (c == ',') {
            bool inline_object =
                (stack_top >= 0 && container_stack[stack_top] == '{' &&
                 inline_object_stack[stack_top]);
            if (inline_object) {
                if (out_len + 3 >= capacity) {
                    capacity *= 2;
                    formated_content = realloc(formated_content, capacity);
                }
                formated_content[out_len++] = ',';
                formated_content[out_len++] = ' ';
            } else {
                size_t needed = 2 + (size_t)level * 2;
                while (out_len + needed + 1 >= capacity) capacity *= 2;
                formated_content = realloc(formated_content, capacity);
                formated_content[out_len++] = ',';
                formated_content[out_len++] = '\n';
                for (int s = 0; s < level * 2; s++) {
                    formated_content[out_len++] = ' ';
                }
            }
            continue;
        }

        if (c == ':') {
            if (out_len + 3 >= capacity) {
                capacity *= 2;
                formated_content = realloc(formated_content, capacity);
            }
            formated_content[out_len++] = ':';
            formated_content[out_len++] = ' ';
            continue;
        }

        if (out_len + 2 >= capacity) {
            capacity *= 2;
            formated_content = realloc(formated_content, capacity);
        }
        formated_content[out_len++] = c;
    }

    formated_content[out_len] = '\0';
    free(simplified);
    free(container_stack);
    free(inline_object_stack);
    return formated_content;
}
