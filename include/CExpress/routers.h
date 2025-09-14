/**
 * @file routers.h
 * @brief Provides structures and functions for managing HTTP routes in the server.
 *
 * This header defines the Router and RouterList structures, HTTP method enumeration,
 * handler function type, and functions for adding, removing, finding, and processing routes.
 * Routes map HTTP methods and paths to handler functions that handle client requests.
 *
 * @author Karl-Alexandre Michaud
 * @date 2025-09-01
 */

#pragma once


#include "utils.h"
#include "handlers.h"


/**
 * @enum method_t
 * @brief Enumeration of supported HTTP methods.
 */
typedef enum {GET, POST, PUT, DELETE, FAIL} method_t;


/**
 * @typedef path_t
 * @brief Alias for representing a URL path.
 */
typedef char * path_t;


/**
 * @struct Router
 * @brief Represents a single HTTP route mapping.
 *
 * A Router links an HTTP method and path to a handler function that processes
 * client requests for that specific route.
 */
typedef struct {
    method_t method;
    path_t path;
    HandlerFunc handler;
} Router;


/**
 * @struct RouterList
 * @brief Represents a dynamic collection of Router objects.
 *
 * This structure manages a list of routes, allowing addition, removal, and searching
 * for specific routes.
 */
typedef struct {
    Router *items;
    size_t count;
    size_t capacity;
} RouterList;


/**
 * @brief Checks if two Router objects are identical.
 *
 * @param a The first Router to compare.
 * @param b The second Router to compare.
 * @return 1 if the routers are the same (same method and path), 0 otherwise.
 */
int same_router(const Router a, const Router b);


/**
 * @brief Adds a Router to the RouterList.
 *
 * @param router_lst Pointer to the RouterList where the route should be added.
 * @param router The Router to add.
 * @return 1 if the route was successfully added, 0 otherwise.
 */
int add_route(RouterList *router_lst, Router router);


/**
 * @brief Removes a Router from the RouterList.
 *
 * @param router_lst Pointer to the RouterList from which the route should be removed.
 * @param router The Router to remove.
 * @return 1 if the route was successfully removed, 0 otherwise.
 */
int remove_route(RouterList *router_lst, Router router);


/**
 * @brief Finds the index of a Router in the RouterList.
 *
 * @param router_lst Pointer to the RouterList to search in.
 * @param router The Router to find.
 * @return The index of the route if found, or -1 if not found.
 */
int find_route(RouterList *router_lst, Router router);


/**
 * @brief Extracts a Router object from an HTTP request header.
 *
 * @param header The raw HTTP request header string.
 * @return A Router object constructed from the header information.
 */
Router extract_router(const char *header);


/**
 * @brief Processes an HTTP request header and dispatches the request to the appropriate route handler.
 *
 * @param header      The raw HTTP request header string.
 * @param client_sock The socket file descriptor of the connected client.
 * @param router_lst  The list of registered routes and their corresponding handlers.
 *
 * @return 1 if the request was successfully processed and handled, 
 *         0 if no matching route was found.
 */
 int process_header(const char *header, int client_sock, RouterList *router_lst);

