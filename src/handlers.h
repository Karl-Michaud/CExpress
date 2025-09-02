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
 * @brief Executes the specified route handler and sends the generated HTTP response to the client.
 *
 * This function:
 * 1. Calls the route's handler function to generate the response body.
 * 2. Constructs an HTTP/1.1-compliant response by adding the appropriate headers using `add_http_header`.
 * 3. Sends the complete response (headers + body) to the client via the provided socket.
 * 4. Frees any dynamically allocated memory used during the process.
 *
 * @param header      The original HTTP request header (optional, can be used for logging or context).
 * @param client_sock The socket file descriptor of the connected client.
 * @param handler     The function pointer to the route handler responsible for generating the response body.
 *
 * @return 1 if the response was successfully sent, 
 *         0 if an error occurred during execution or sending.
 */
int execute_handler(const char *header, int client_sock, HandlerFunc handler);
