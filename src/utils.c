#include "utils.h"


char **extract_lines(const char *buffer, size_t buff_size) {
    if (!buffer || buff_size == 0) {
        return NULL;
    }

    // Exponential list re-sizing approach:
    // Start with a list of length 4, and double the size any time the list is full.
    // Amortized cost is O(n).
    size_t capacity = 4;
    size_t count = 0;
    size_t line_start = 0;
    int header_end = 0;
    char **lines = malloc(capacity * sizeof(char *));

    if (!lines) {
        perror("malloc failed. Lines can't be extracted.");
        return NULL;
    }

    for (size_t i = 0; i < buff_size - 1; i++) {
        if (buffer[i] == '\r' && buffer[i + 1] == '\n') {
            size_t len_line = i - line_start;
            // Check for blank line at the end of the HTTP header
            if (len_line == 0) {                // blank line -> end of header block
                header_end = 1;
                break;
            }

            char *line = malloc(sizeof(char) * len_line + 1); // +1 for null terminator
            if (!line) {
                perror("malloc failed. Lines can't be extracted.");
                goto fail;
            }
            memcpy(line, buffer + line_start, len_line);
            line[len_line] = '\0';
    
            if (count == capacity) {
                size_t new_cap = capacity * 2;
                char **temp = realloc(lines, new_cap * sizeof(char *));
                if (!temp) {
                    free(line);
                    perror("malloc failed. Lines can't be extracted.");
                    goto fail;
                }
                lines = temp;
                capacity = new_cap;
            }
            lines[count] = line;
            count++;
            
            i++; // Move past `\n` since it will be incremented again by loop.
            line_start = i + 1;
        }
    }

    // If we didn't hit the header terminator, handle a trailing partial line
    if (!header_end && line_start < buff_size) {
        size_t len_line = buff_size - line_start;
        char *line = malloc(len_line + 1);
        if (!line) { 
            perror("malloc trailing"); goto fail; 
        }

        memcpy(line, buffer + line_start, len_line);
        line[len_line] = '\0';

        if (count == capacity) {
            size_t newcap = capacity * 2;
            char **temp = realloc(lines, newcap * sizeof(char *));
            if (!temp) {
                free(line);
                perror("realloc lines");
                goto fail;
            }
            lines = temp;
            capacity = newcap;
        }
        lines[count++] = line;
    }

    // Shrink to real size and NULL-terminate array
    {
        char **temp = realloc(lines, (count + 1) * sizeof *lines);
        if (temp) {
            lines = temp; // if realloc fails, keep original (still valid)
        }
        lines[count] = NULL;
    }
    return lines;

fail:
    for (size_t k = 0; k < count; ++k) {
        free(lines[k]);
    };
    free(lines);
    return NULL;
}

