/**
 * @file server.h
 * @brief Defines the Server struct, client struct, and server-related function
 * prototypes for creating, initializing, and managing a TCP server.
 *
 * Includes:
 *   - Standard headers for networking and system calls
 *   - Utility headers
 *
 * Usage:
 *   Include this file in your source files that need to interact with
 *   the server, e.g., server.c or main.c.
 *
 * @author Karl-Alexandre Michaud
 * @date 2025-08-29
 */


#pragma once

// Include all necessary headers for server
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h> // Necessary for handling signals
#include <errno.h>    // defines errno, EINTR, EAGAIN, EWOULDBLOCK

#include "utils.h"
#include "routers.h"

// User defined constants
#define BUFFER_SIZE 1024



/**
 * @struct client_t
 * @brief Represents a connected client.
 */
typedef struc
typedef struct {
    int client_sock;          // client socket
    struct sockaddr_in addr;  // client address
    RouterList router_lst     // routing list
} client_t;


/**
 * @struct Server
 * @brief Represents the TCP server configuration and state.
 */
typedef struct {
    int sockfd;               // server socket
    struct sockaddr_in addr;  // server address
    int port;                 // server port
    int max_clients;          // server max amount of concurrent clients
    client_t *client_lst;     // list of connected clients
    int backlog;              // max number of partially completed connections (queue for clients)             
} Server;



/**
 * @brief Initializes a new server instance.
 *
 * Allocates memory for the server and prepares it for binding and listening.
 *
 * @param port The TCP port number the server should bind to.
 * @param max_clients Maximum number of concurrent client connections allowed.
 * @param backlog Maximum number of pending connections in the listen queue.
 * @return Pointer to a dynamically allocated Server struct on success, or NULL on failure.
 *
 * @note Use server_free() to properly shut down and free allocated memory.
 */
Server *server_init(int port, int max_clients, int backlog);


/**
 * @brief Frees all server resources and shuts down the server.
 *
 * Closes the socket and deallocates any memory used by the server instance.
 *
 * @param server Pointer to the Server struct to be freed.
 * @return 0 on success, -1 on failure (e.g., invalid pointer).
 */
int server_free(Server *server);


/**
 * @brief Starts the server and begins accepting client connections.
 *
 * This function binds the socket, listens for incoming connections,
 * and handles client requests in a loop.
 *
 * @param server Pointer to the initialized Server struct.
 * @return 0 on successful start, or -1 on error.
 */
int server_start(Server *server);


/**
 * @brief Adds a new route to the client's router list for handling HTTP requests.
 *
 * This function associates an HTTP method and path with a handler function.
 * If the router list is full, it will automatically resize the list.
 *
 * @param routers Pointer to the RouterList in which to add the new route.
 * @param method The HTTP method (GET, POST, PUT, DELETE) for this route.
 * @param path The URL path string for this route.
 * @param handler Function pointer that will handle requests matching method and path.
 *
 * @return 1 on success, 0 on failure (e.g., memory allocation failed).
 */
int client_add_route(RouterList *routers, method_t method, path_t path, HandlerFunc handler);


/**
 * @brief Removes a route from the client's router list.
 *
 * Deletes the route at the given index and shifts subsequent routes down.
 *
 * @param router_lst Pointer to the RouterList from which the route will be removed.
 * @param index The index of the route to remove.
 *
 * @return 1 on success, 0 if index is invalid or removal fails.
 */
int client_remove_route(RouterList *router_lst, size_t index);

