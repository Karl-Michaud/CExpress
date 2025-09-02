#include "handlers.h"


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
char *execute_handler(const char *header, client_t *client, Router *router) {
    if (!router || !router->handler) return NULL;

    // Call the handler; it returns a dynamically allocated string
    char *handler_str = router->handler();
    if (!handler_str) return NULL;

    // Construct full HTTP/1.1 response
    char *response = add_http_header(handler_str, strlen(handler_str));

    // Free the handler's string to prevent memory leaks
    free(handler_str);

    return response;
}

