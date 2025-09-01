#pragma once

#include "utils.h"


/**
 * @typedef HandlerFunc
 * @brief Function pointer type for handling client requests.
 *
 * @param client_sock The client socket descriptor.
 * @param request The raw HTTP request string.
 */
typedef void (*HandlerFunc)(int client_sock, const char *request);


