#include "handlers.h"


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
int execute_handler(const char *header, client_t *client, Router *router) {
    if (!router || !router->handler) return 0;

    // Call the handler
    char *handler_str = router->handler();
    if (!handler_str) return 0;

    // Construct HTTP/1.1 response
    char *response = add_http_header(handler_str, strlen(handler_str));
    free(handler_str);

    if (!response) return 0;
    
    // Send response to client
    ssize_t sent = write(client->client_sock, response, strlen(response));
    free(response);

    return (sent > 0) ? 1 : 0;
}

