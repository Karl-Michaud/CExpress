/**
 * @file utils.h
 * @brief Utility functions for string manipulation and HTTP header parsing.
 *
 * This header provides helper functions to process and extract data from HTTP requests,
 * such as splitting strings, extracting key-value pairs, and retrieving individual lines
 * from an HTTP header.
 *
 * @author Karl-Alexandre Michaud
 * @date 2025-08-30
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>



/**
 * @brief Extracts all key-value pairs from an HTTP header buffer.
 *
 * Matches lines of the form:
 *     Key: Value
 *
 * @param buffer The input string containing the HTTP header.
 * @return A NULL-terminated array of strings, each representing a "key:value" pair.
 *         The caller must free the allocated memory.
 */
char **extract_key_value(const char *buffer);



/*
 * @brief Splits a string into tokens based on a specified separator.
 *
 * @param buffer The input string to split.
 * @param buff_size The size of the buffer.
 * @param sep The character separator to use for splitting.
 * @return A NULL-terminated array of strings containing the tokens.
 *         The caller must free the allocated memory.
 */
char **split(const char *buffer, size_t buff_size, const char sep);




/**
 * @brief Extracts individual lines from an HTTP header buffer.
 *
 * This function is currently designed for HTTP/1.1 headers, where lines are
 * separated by "\r\n".
 *
 * @param buffer The input string containing the HTTP header.
 * @param buff_size The size of the buffer.
 * @return A NULL-terminated array of strings, each representing a line from the header.
 *         The caller must free the allocated memory.
 */
char **extract_lines(const char *buffer, size_t buff_size);

