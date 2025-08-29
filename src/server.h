#pragma once

// Include all necessary headers for server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "utils.h"

// User defined constants
#define BUFFER_SIZE 1024

typedef struct {
    int sockfd;               // server socket
    struct sockaddr_in addr;  // server address
    int port;                 // server port
    int max_clients;          // server max amount of concurrent clients
    fd_set sock_set           // all client/server sockets being monitored by server (used for select() sys call)
    int max_fd                // biggest file descriptor (used for select() sys call)
                              // Future: dynamic route table
} Server;


/*
 * Initialize a new server instance.
 * - `port`: the server port number
 * - `max_clients`: the max number of concurrent clients
 *
 * NOTE: Call server_free() to shut down server and free allocated memory.
 */
Server *server_init(int port, int max_clients);

/*
 * Free server resources. This, by extension, kills the server.
 * - `server`: pointer to Server struct
 */
int server_free(Server *server);

/*
 * Start listening and handling requests.
 * - `server`: pointer to Server stuct
 */
int server_start(Server *server);


/*
 * Add a route to the server.
 * - `server`: pointer to Server struct
 * - `method`: type of method (e.g.: "GET", "POST", etc.)
 * - `path`: url path
 * - `handler`: function pointer to user defined handler function
 */
int server_add_route(Server *server, method_t method, path_t path, HandlerFunc handler);

