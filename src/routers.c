/**
 * @file router.c
 * @brief Manages routing for HTTP-like requests in the CExpress framework.
 *
 * This file provides the core logic for creating, adding, removing, and
 * searching routes. It also includes utility functions for extracting
 * a Router from an HTTP header and processing requests against the
 * registered routes.
 *
 * @author Karl-Alexandre Michaud
 * @date 2025-09-01
 */

#include "routers.h"


/**
 * @brief Compares two Router structs for equality.
 *
 * Equality is determined by matching both the HTTP method and the path.
 *
 * @param a First Router to compare.
 * @param b Second Router to compare.
 * @return 1 if both routers are equal, 0 otherwise.
 */
int same_router(const Router a, const Router b) {
    return (a.method == b.method) && (strcmp(a.path, b.path) == 0);
}


/**
 * @brief Adds a new Router to the RouterList.
 *
 * Expands the list if the capacity is reached. Uses dynamic memory management.
 *
 * @param router_lst Pointer to the RouterList to add to.
 * @param router Router to add.
 * @return 1 on success, 0 on failure (e.g., realloc failure).
 */
int add_route(RouterList *router_lst, Router router) {
    if (router_lst->count == router_lst->capacity) {
        size_t new_cap = router_lst->capacity * 2;
        Router *temp = realloc(router_lst->items, new_cap * sizeof(Router));
        if (!temp) {
            perror("realloc failed. Route not added.");
            return 0;
        }
        router_lst->items = temp;
        router_lst->capacity = new_cap;
    }
    router_lst->items[router_lst->count] = router;
    router_lst->count++;
    return 1;
}


/**
 * @brief Removes a Router from the RouterList.
 *
 * Shifts elements to fill the removed slot and clears the last element.
 *
 * @param router_lst Pointer to the RouterList.
 * @param router Router to remove.
 * @return 1 if removed successfully, 0 if router not found.
 */
int remove_route(RouterList *router_lst, Router router) {
    for (size_t i = 0; i < router_lst->count; i++) {
        if (same_router(router_lst->items[i], router)) {
            // Shift left all items after i
            for (size_t j = i; j < router_lst->count - 1; j++) {
                router_lst->items[j] = router_lst->items[j + 1];
            }
            router_lst->count--;
            
            // Clear last element to avoid stale data
            memset(&router_lst->items[router_lst->count], 0, sizeof(Router));
            
            return 1;
        }
    }
    return 0;
}


/**
 * @brief Searches for a Router in a RouterList.
 *
 * @param router_lst Pointer to the RouterList.
 * @param router Router to search for.
 * @return index if found, -1 otherwise.
 */
int find_route(RouterList *router_lst, Router router) {
    for (size_t i = 0; i < router_lst->count; i++) {
        if (same_router(router_lst->items[i], router)) {
            return i;
        }
    }
    return -1;
}



/**
 * @brief Extracts a Router from an HTTP header string.
 *
 * Parses the first line of the HTTP request to determine the method and path.
 *
 * @param header Pointer to the HTTP request string.
 * @return Router struct with method and path set. If parsing fails, returns a Router with all fields zeroed.
 */
Router extract_router(const char *header) {
    char **extracted_lines = extract_lines(header, strlen(header));
    if (!extracted_lines) {
        Router temp_router;
        temp_router.method = FAIL;
        temp_router.path = NULL;
        temp_router.handler = NULL;
        return temp_router;
    }

    char **header_sep = split(extracted_lines[0], strlen(extracted_lines[0]), ' ');
    if (!header_sep) {
       for (size_t i = 0; extracted_lines[i] != NULL; i++) {
           free(extracted_lines[i]);
       } 
       free(extracted_lines);

       Router temp_router;
       temp_router.method = FAIL;
       temp_router.path = NULL;
       temp_router.handler = NULL;
       return temp_router;
    }
    
    Router router;
    memset(&router, 0, sizeof(Router)); // Get rid of previous data

    const char *method = header_sep[0];
    // typedef enum {GET, POST, PUT, DELETE} method_t;
    if (strcmp(method, "GET") == 0) {
        router.method = GET;
    } else if (strcmp(method, "POST") == 0) {
        router.method = POST;
    } else if (strcmp(method, "PUT") == 0) {
        router.method = PUT;
    } else if (strcmp(method, "DELETE") == 0) {
        router.method = DELETE;
    }

    router.path = strdup(header_sep[1]);
    
    // Free all memory
    for (size_t i = 0; extracted_lines[i] != NULL; i++) {
        free(extracted_lines[i]);
    }
    free(extracted_lines);
    for (size_t i = 0; header_sep[i] != NULL; i++) {
        free(header_sep[i]);
    }
    free(header_sep);

    return router;
}


/**
 * @brief Processes an HTTP request header and attempts to execute the corresponding route handler.
 *
 * Parses the HTTP header to extract the method and path, searches for a matching route in the RouterList,
 * and invokes the associated handler if a match is found.
 *
 * @param header      The raw HTTP request header string.
 * @param client_sock The socket file descriptor of the connected client.
 * @param router_lst  The list of registered routes and their corresponding handlers.
 *
 * @return 1 if a matching route was found and its handler was executed successfully, 
 *         0 if no matching route exists or the header is invalid.
 */
int process_header(const char *header, int client_sock, RouterList *router_lst) {
    Router extracted_router = extract_router(header);
    if (extracted_router.method == FAIL) {
        return 0;
    }
    
    int index = find_route(router_lst, extracted_router);
    if (index == -1) {
        // router not found
        return 0;
    }
    
    return execute_handler(header, client_sock, router_lst->items[index].handler); 
}
