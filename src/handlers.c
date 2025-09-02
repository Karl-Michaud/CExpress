#include "handlers.h"


/**
 * @brief Executes the specified route handler and sends an HTTP response to the client.
 *
 * This function:
 * 1. Invokes the route's handler function to generate the response body.
 * 2. Constructs a valid HTTP/1.1 response by adding headers via `add_http_header`.
 * 3. Sends the complete response (headers + body) through the client's socket.
 * 4. Frees any dynamically allocated memory used in the process.
 *
 * @param header      The original HTTP request header (optional, useful for logging or debugging).
 * @param client_sock The socket file descriptor of the connected client.
 * @param handler     The function pointer to the route handler responsible for generating the response body.
 *
 * @return 1 if the response was successfully sent,
 *         0 if an error occurred while executing the handler or sending the response.
 */
int execute_handler(const char *header, int client_sock, HandlerFunc handler) {
    if (client_sock == -1 || !handler) return 0;

    // Call the handler
    char *handler_str = handler();
    if (!handler_str) return 0;

    // Construct HTTP/1.1 response
    char *response = add_http_header(handler_str, strlen(handler_str));
    free(handler_str);

    if (!response) return 0;
    
    // Send response to client
    ssize_t sent = write(client_sock, response, strlen(response));
    free(response);

    return (sent > 0) ? 1 : 0;
}

