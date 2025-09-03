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

#include "../../src/utils.h"
#include "../../src/routers.h"

// User defined constants
#define BUFFER_SIZE 1024
#define LOCALHOST_IP "127.0.0.1"


/**
 * @enum Mode
 * @brief Specifies the server binding mode.
 *
 * This enumeration determines how the server socket binds to the host.
 * - DEV  : Bind to localhost (127.0.0.1), restricting access to the local machine.
 * - PROD : Bind to all interfaces (0.0.0.0), allowing external network access.
 */
typedef enum { DEV, PROD } Mode;


/**
 * @struct client_t
 * @brief Represents a connected client.
 */
typedef struct {
    int client_sock;          // client socket
    struct sockaddr_in addr;  // client address
} client_t;


/**
 * @struct Server
 * @brief Represents the TCP server configuration and state.
 */
typedef struct {
    int sockfd;               // server socket
    struct sockaddr_in addr;  // server address
    int port;                 // server port
    Mode mode;
    int max_clients;          // server max amount of concurrent clients
    client_t *client_lst;     // list of connected clients
    int backlog;              // max number of partially completed connections (queue for clients)
    RouterList router_lst;    // global routing list
} Server;



/**
 * @brief Initializes a new server instance.
 *
 * Allocates memory for the server and prepares it for binding and listening.
 *
 * @param port The TCP port number the server should bind to.
 * @param max_clients Maximum number of concurrent client connections allowed.
 * @param backlog Maximum number of pending connections in the listen queue.
 * @param mode Server binding mode:
 *        - DEV  : Bind to localhost (127.0.0.1), restricting access to the local machine.
 *        - PROD : Bind to all interfaces (0.0.0.0), allowing external network access.
 *
 * @return Pointer to a dynamically allocated Server struct on success, or NULL on failure.
 *
 * @note Use server_free() to properly shut down and free allocated memory.
 */
Server *server_init(int port, int max_clients, int backlog, Mode mode);


/**
 * @brief Frees all server resources and shuts down the server.
 *
 * Closes the socket and deallocates any memory used by the server instance.
 *
 * @param server Pointer to the Server struct to be freed.
 */
void server_free(Server *server);


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
 * @brief Registers a new route in the server's RouterList for handling HTTP requests.
 *
 * This function associates an HTTP method and path with a specific handler function.
 * If the RouterList is at capacity, it automatically resizes to accommodate the new route.
 *
 * @param server  Pointer to the Server instance where the route will be added.
 * @param method  The HTTP method (e.g., GET, POST, PUT, DELETE) for the route.
 * @param path    The URL path string for the route.
 * @param handler The function pointer to the handler that processes requests matching the method and path.
 *
 * @return 1 on success,
 *         0 on failure (e.g., memory allocation error or invalid parameters).
 */
int server_add_route(Server *server, method_t method, path_t path, HandlerFunc handler);


/**
 * @brief Unregisters a route from the server's RouterList.
 *
 * This function searches for the route matching the specified HTTP method and path,
 * and removes it if found. The RouterList is compacted by shifting remaining routes down.
 *
 * @param server Pointer to the Server instance from which the route will be removed.
 * @param method The HTTP method of the route to remove.
 * @param path   The URL path of the route to remove.
 *
 * @return 1 if the route was successfully removed,
 *         0 if the route does not exist or an error occurred.
 */
int server_remove_route(Server *server, method_t method, path_t path);

