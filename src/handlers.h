/**
 * @file handler.h
 * @brief Defines HTTP request handlers and execution utilities for the server.
 *
 * Provides the type definition for handler functions and a function to execute
 * a handler given the HTTP header and client context.
 *
 * @author Karl-Alexandre Michaud
 * @date 2025-09-01
 */


#pragma once

#include "utils.h"


/**
 * @typedef HandlerFunc
 * @brief Function pointer type for handling HTTP requests.
 *
 * This function is called when a request matches a route.
 * 
 * @return A dynamically allocated string (usually JSON or HTML) representing the response body.
 *         The caller (server/framework) is responsible for freeing this string after use.
 *
 * @note The handler takes no parameters. All necessary information should already be available 
 *       in the context in which it is executed.
 */
typedef char *(*HandlerFunc)(void);


/**
 * @brief Executes the handler associated with a route and constructs an HTTP response.
 *
 * This function:
 * 1. Calls the provided route's handler function.
 * 2. Wraps the returned string in a proper HTTP/1.1 response header using `add_http_header`.
 * 3. Frees the string returned by the handler to remove the burden from the caller.
 *
 * @param header   The original request header (used for logging or future extensions, can be NULL).
 * @param client   Pointer to the client structure (currently unused but available for context).
 * @param router   Pointer to the route containing the handler to execute.
 *
 * @return A dynamically allocated string containing the full HTTP/1.1 response. 
 *         The caller is responsible for freeing this string using `free()`.
 */
char *execute_handler(const char *header, client_t *client, Router *router);
