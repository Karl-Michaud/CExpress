#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


typedef enum {GET, POST, PUT, DELETE} method_t;

typedef char * path_t;

typedef void (*HandlerFunc)(int client_sock, const char *request);


/*
 * 
 */
char **split(char *str);


/*
 * Extract lines from HTTP header.
 *
 * Note: Currently, this is only compatible for HTTP 1.1 headers.
 */
char **extract_lines(const char *buffer, size_t buff_size);
