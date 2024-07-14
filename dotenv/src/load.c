#include "api.h"

#include <stdio.h>
#include <stdlib.h>

int dotenv_load_default() { return dotenv_load(DOTENV_DEFAULT_ENV_FILE_NAME); }

enum ParsingError { NONE, SKIP, CRITICAL };

struct ParsedLine {
    char const *name;
    char const *value;
    enum ParsingError error;
};

static inline struct ParsedLine *parse_env_line(char *line, size_t len);

int dotenv_load(char const *env_file) {
    FILE *env_file_fd = fopen(env_file, "r");
    if (env_file_fd == NULL) {
        return -1;
    }

    char *line = NULL;
    size_t line_len = 0;

    size_t bytes_read;
    while ((bytes_read = getline(&line, &line_len, env_file_fd)) != -1) {
        struct ParsedLine *parsed_line = parse_env_line(line, bytes_read);

        // Critical error: couldn't allocate mem with malloc
        if (parsed_line == NULL) {
            return -1;
        }

        switch (parsed_line->error) {
        case NONE:
            setenv(parsed_line->name, parsed_line->value,
                   DOTENV_ALWAYS_OVERWRITE);
            break;
        case SKIP:
            break;
        case CRITICAL:
            free(parsed_line);
            return -1;
        }

        free(parsed_line);
    }

    fclose(env_file_fd);

    if (line != NULL) {
        free(line);
    }

    return 0;
}

enum EnvParserState {
    WAINTING_FOR_CHAR,
    IGNORING_START_WHITESPACES,
    READING_NAME,
    READING_VALUE,
    IGNORING_END_WHITESPACES,
    ERROR,
};

enum CharType {
    EQUAL_SIGN,
    WHITESPACE,
    CHARACTER,
};

static inline enum CharType define_char_type(char c) {
    if (c == '=') {
        return EQUAL_SIGN;
    }

    if (c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\v') {
        return WHITESPACE;
    }

    return CHARACTER;
}

static inline struct ParsedLine *parse_env_line(char *line, size_t len) {
    static enum EnvParserState transfer[6][3] = {
        {ERROR, IGNORING_START_WHITESPACES, READING_NAME},
        {ERROR, IGNORING_START_WHITESPACES, READING_NAME},
        {READING_VALUE, ERROR, READING_NAME},
        {READING_VALUE, IGNORING_END_WHITESPACES, READING_VALUE},
        {ERROR, IGNORING_END_WHITESPACES, ERROR},
        {ERROR, ERROR, ERROR}};

    struct ParsedLine *result = malloc(sizeof(struct ParsedLine));
    if (result == NULL) {
        return NULL;
    }
    result->error = NONE;

    // name=value
    // ^   ^     ^
    // 1   2     3
    //
    // 1 - start
    // 2 - separator_pos
    // 3 - end
    size_t start = 0;
    size_t separator_pos = 0;
    size_t end = 0;

    enum EnvParserState prev_state = WAINTING_FOR_CHAR;
    enum EnvParserState state = WAINTING_FOR_CHAR;
    for (size_t i = 0; i < len && state != ERROR; i++) {
        char c = *(line + i);
        enum CharType type = define_char_type(c);

        prev_state = state;
        state = transfer[state][type];

        if (prev_state == IGNORING_START_WHITESPACES && state == READING_NAME) {
            start = i;
        } else if (prev_state == READING_NAME && state == READING_VALUE) {
            separator_pos = i;
        } else if (prev_state == READING_VALUE &&
                   state == IGNORING_END_WHITESPACES) {
            end = i;
        }
    }

    if (end == 0) {
        end = len;
    }

    switch (state) {
    case WAINTING_FOR_CHAR:
    case IGNORING_START_WHITESPACES:
        result->error = SKIP; // just an empty line => ignore it
        break;

    case READING_NAME:
        result->error = CRITICAL; // syntax error
        break;

    case READING_VALUE:
    case IGNORING_END_WHITESPACES:
        line[separator_pos] = '\0';
        line[end] = '\0';
        result->name = line + start;
        result->value = line + separator_pos + 1;
        break;

    case ERROR:
        result->error = CRITICAL;
    }

    return result;
}
