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
 * @brief Executes the handler for a route and sends the HTTP response to the client.
 *
 * This function:
 * 1. Calls the route's handler to get the response body.
 * 2. Wraps it in a proper HTTP/1.1 response header using `add_http_header`.
 * 3. Sends the full response to the client's socket.
 * 4. Frees all dynamically allocated memory internally.
 *
 * @param header The original request header (can be used for logging, optional).
 * @param client Pointer to the client structure containing the socket.
 * @param router Pointer to the route containing the handler to execute.
 *
 * @return 1 on successful send, 0 on failure.
 */
int execute_handler(const char *header, client_t *client, Router *router);
