#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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
        } else if (*source != ' ' && *source != '\n' && *source != '\t' &&
                   *source != '\r') {
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

static bool is_hex_digit(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

static bool parse_string(const char* s, size_t* index) {
    if (s[*index] != '"') return false;
    (*index)++;
    while (s[*index]) {
        char c = s[*index];
        if (c == '"') {
            (*index)++;
            return true;
        }
        if ((unsigned char)c < 0x20) return false;
        if (c == '\\') {
            (*index)++;
            char esc = s[*index];
            if (!esc) return false;
            if (esc == '"' || esc == '\\' || esc == '/' || esc == 'b' ||
                esc == 'f' || esc == 'n' || esc == 'r' || esc == 't') {
                (*index)++;
                continue;
            }
            if (esc == 'u') {
                for (int i = 0; i < 4; i++) {
                    if (!is_hex_digit(s[*index + 1 + i])) return false;
                }
                *index += 5;
                continue;
            }
            return false;
        }
        (*index)++;
    }
    return false;
}

static bool parse_number(const char* s, size_t* index) {
    size_t i = *index;
    if (s[i] == '-') i++;
    if (s[i] == '0') {
        i++;
    } else {
        if (s[i] < '1' || s[i] > '9') return false;
        while (s[i] >= '0' && s[i] <= '9') i++;
    }
    if (s[i] == '.') {
        i++;
        if (s[i] < '0' || s[i] > '9') return false;
        while (s[i] >= '0' && s[i] <= '9') i++;
    }
    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        if (s[i] == '+' || s[i] == '-') i++;
        if (s[i] < '0' || s[i] > '9') return false;
        while (s[i] >= '0' && s[i] <= '9') i++;
    }
    *index = i;
    return true;
}

static bool parse_literal(const char* s, size_t* index, const char* literal) {
    size_t len = strlen(literal);
    if (strncmp(s + *index, literal, len) != 0) return false;
    *index += len;
    return true;
}

static bool parse_value(const char* s, size_t* index);

static bool parse_array(const char* s, size_t* index) {
    if (s[*index] != '[') return false;
    (*index)++;
    if (s[*index] == ']') {
        (*index)++;
        return true;
    }
    while (true) {
        if (!parse_value(s, index)) return false;
        if (s[*index] == ',') {
            (*index)++;
            continue;
        }
        if (s[*index] == ']') {
            (*index)++;
            return true;
        }
        return false;
    }
}

static bool parse_object(const char* s, size_t* index) {
    if (s[*index] != '{') return false;
    (*index)++;
    if (s[*index] == '}') {
        (*index)++;
        return true;
    }
    while (true) {
        if (!parse_string(s, index)) return false;
        if (s[*index] != ':') return false;
        (*index)++;
        if (!parse_value(s, index)) return false;
        if (s[*index] == ',') {
            (*index)++;
            continue;
        }
        if (s[*index] == '}') {
            (*index)++;
            return true;
        }
        return false;
    }
}

static bool parse_value(const char* s, size_t* index) {
    char c = s[*index];
    if (c == '"') return parse_string(s, index);
    if (c == '{') return parse_object(s, index);
    if (c == '[') return parse_array(s, index);
    if (c == 't') return parse_literal(s, index, "true");
    if (c == 'f') return parse_literal(s, index, "false");
    if (c == 'n') return parse_literal(s, index, "null");
    if (c == '-' || (c >= '0' && c <= '9')) return parse_number(s, index);
    return false;
}

int validate_json(const char* json_content) {
    if (!json_content) return 0;
    char* simplified = simplify_json(json_content);
    if (!simplified) return 0;
    size_t index = 0;
    bool ok = parse_value(simplified, &index) && simplified[index] == '\0';
    free(simplified);
    return ok ? 1 : 0;
}
