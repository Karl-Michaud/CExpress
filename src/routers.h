#pragma once

#include "utils.h"


/**
 * @enum method_t
 * @brief Enumeration of supported HTTP methods.
 */
typedef enum {GET, POST, PUT, DELETE} method_t;


/**
 * @typedef path_t
 * @brief Alias for representing a URL path.
 */
typedef char * path_t;


/**
 * @typedef HandlerFunc
 * @brief Function pointer type for handling client requests.
 *
 * @param client_sock The client socket descriptor.
 * @param request The raw HTTP request string.
 */
typedef void (*HandlerFunc)(int client_sock, const char *request);


typedef struct {
    method_t method;
    path_t path;
    HandlerFunc handler;
} Router;

typedef struct {
    Router *items;
    size_t count;
    size_t capacity;
} RouterList;


int same_router(const Router a, const Router b)


int add_route(RouterList *router_lst, Router router);


int remove_route(RouterList *router_lst, Router router);


int find_route(RouterList *router_lst, Router router);


Router extract_router(const char *header);


int process_header(const char *header, RouterList *router_lst)
