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

#include "router.h"


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
 * @return 1 if found, 0 otherwise.
 */
int find_route(RouterList *router_lst, Router router) {
    for (size_t i = 0; i < router_lst->count; i++) {
        if (same_router(router_lst->items[i], router)) {
            return 1;
        }
    }
    return 0;
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
    char **extracted_lines = extracted_lines(header, strlen(header));
    if (!extracted_lines) {
        return NULL;
    }

    char **header_sep = split(extracted_lines[0], strlen(extracted_lines[0]), ' ');
    if (!header_sep) {
       for (size_t i = 0; extracted_lines[i] != NULL; i++) {
           free(extracted_lines[i]);
       } 
       free(extracted_lines);
       return NULL;
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

    router.path = header_sep[1];
    
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
 * @brief Processes an HTTP header against the RouterList.
 *
 * Extracts the router from the header and checks if it exists in the list.
 * If found, executes the corresponding handler (TODO: implement).
 *
 * @param header Pointer to the HTTP request string.
 * @param client Pointer to the concerned client.
 * @return 1 if the route exists and processing can continue, 0 otherwise.
 */
int process_header(const char *header, client_t *client) {
    Router extracted_router = extract_router(header);
    if (!extracted_router) return 0;

    if (!find_route(router_lst, router)) {
        // router not found. abort.
        return 0;
    }

    // TODO: execute method for given route
    
    return 1;
}

