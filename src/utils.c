/**
 * @file utils.c
 * @brief Implementation of utility functions for HTTP header parsing and string operations.
 *
 * This file provides the implementation of helper functions declared in utils.h,
 * including splitting strings, extracting key-value pairs from HTTP headers,
 * and extracting individual lines from HTTP requests.
 *
 * @author Karl-Alexandre Michaud
 * @date 2025-08-31
 */


#include "utils.h"




/**
 * @brief Extracts a key-value pair from a single header line.
 *
 * This function uses a regular expression to parse a line of the form:
 *     Key: Value
 * and returns a dynamically allocated array containing two elements:
 *     - result[0]: the key (string)
 *     - result[1]: the value (string)
 *     - result[2]: NULL (for safety)
 *
 * @param line The input string containing the header line.
 * @return A NULL-terminated array of two strings (key and value) or NULL on failure.
 *         Caller must free all allocated memory.
 */
char **extract_key_value(const char *line) {
    regex_t regex;
    regmatch_t matches[3]; // 0: full match, 1: key, 2: value
    const char *pattern = "^([!#$%&'*+.^_`|~0-9A-Za-z-]+):[ \t]*(.+)$";

    // Compile regex
    if (regcomp(&regex, pattern, REG_EXTENDED)) {
        fprintf(stderr, "Could not compile regex\n");
        return NULL;
    }

    int status = regexec(&regex, line, 3, matches, 0);
    if (status != 0) {
        regfree(&regex);
        return NULL; // no match
    }

    // Calculate lengths
    size_t key_len = matches[1].rm_eo - matches[1].rm_so;
    size_t val_len = matches[2].rm_eo - matches[2].rm_so;

    // Allocate char** (2 elements + NULL terminator)
    char **result = malloc(3 * sizeof(char *));
    if (!result) {
        regfree(&regex);
        return NULL;
    }

    // Allocate and copy key
    result[0] = malloc(key_len + 1);
    strncpy(result[0], line + matches[1].rm_so, key_len);
    result[0][key_len] = '\0';

    // Allocate and copy value
    result[1] = malloc(val_len + 1);
    strncpy(result[1], line + matches[2].rm_so, val_len);
    result[1][val_len] = '\0';

    result[2] = NULL; // Optional NULL terminator for safety

    regfree(&regex);
    return result;
}


/**
 * @brief Splits a string into tokens using a single character separator.
 *
 * This function dynamically allocates an array of strings containing the tokens.
 * Uses an exponential resizing approach for efficiency.
 *
 * @param buffer The input string to split.
 * @param buff_size The length of the buffer.
 * @param sep The separator character.
 * @return A NULL-terminated array of strings or NULL on failure.
 *         Caller must free each string and the array.
 */
char **split(const char *buffer, size_t buff_size, const char sep) {
    if (!buffer || buff_size == 0) {
        return NULL;
    }

     // Exponential list re-sizing approach:
    // Start with a list of length 4, and double the size any time the list is full.
    // Amortized cost is O(n).
    size_t capacity = 4;
    size_t count = 0;
    size_t wrd_start = 0;
    char **split_lst = malloc(capacity * sizeof(char *));

    if (!split_lst) {
        perror("malloc failed. Split aborted.");
        return NULL;
    }

    for (size_t i = 0; i < buff_size; i++) {
        if (buffer[i] == sep) {
            size_t len_wrd = i - wrd_start;
            char *wrd = malloc(sizeof(char) * len_wrd + 1); 
            if (!wrd) {
                perror("malloc failed. Split aborted.");
                goto fail;
            }
            memcpy(wrd, buffer + wrd_start, len_wrd);
            wrd[len_wrd] = '\0';

            if (count == capacity) {
                size_t new_cap = capacity * 2;
                char **temp = realloc(split_lst, new_cap * sizeof(char *));
                if (!temp) {
                    free(wrd);
                    perror("malloc failed. Split aborted.");
                    goto fail;
                }
                split_lst = temp;
                capacity = new_cap;
            }
            split_lst[count] = wrd;
            count++;

            wrd_start = i + 1;
        }
    }
    if (wrd_start < buff_size) {
        size_t len_wrd = buff_size - wrd_start;
        char *wrd = malloc(len_wrd + 1);
        if (!wrd) {
            perror("malloc failed. Split aborted.");
            goto fail;
        }
        memcpy(wrd, buffer + wrd_start, len_wrd);
        wrd[len_wrd] = '\0';

        if (count == capacity) {
            size_t new_cap = capacity * 2;
            char **temp = realloc(split_lst, new_cap * sizeof(char *));
            if (!temp) {
                free(wrd);
                perror("malloc failed. Split aborted.");
                goto fail;
            }
            split_lst = temp;
            capacity = new_cap;
        }
        split_lst[count++] = wrd;
    }

    // Shrink to real size and NULL-terminate array
    {
        char **temp = realloc(split_lst, (count + 1) * sizeof(char *));
        if (temp) {
            split_lst = temp; // if realloc fails, keep original (still valid)
        }
        split_lst[count] = NULL;
    }
    return split_lst;

fail:
    for (size_t k = 0; k < count; ++k) {
        free(split_lst[k]);
    };
    free(split_lst);
    return NULL;


}


/**
 * @brief Extracts individual lines from an HTTP header.
 *
 * This function scans the HTTP header and splits it into lines separated by "\r\n".
 * Stops processing at the blank line indicating the end of the header.
 *
 * @param buffer The input string containing the HTTP header.
 * @param buff_size The length of the buffer.
 * @return A NULL-terminated array of strings, each representing one line of the header.
 *         Caller must free each string and the array.
 */
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
        char **temp = realloc(lines, (count + 1) * sizeof(char *));
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

