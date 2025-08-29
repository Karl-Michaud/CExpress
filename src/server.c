/**
 * @file server.c
 * @brief Implements the core server functionality for the CExpress framework.
 *
 * This file contains the implementation of the Server structure and its
 * associated functions that handle server initialization, socket configuration,
 * listening for client connections, and managing HTTP-like routes.
 *
 * Responsibilities:
 *  - Initialize and configure the server socket.
 *  - Start listening for incoming connections on the specified port.
 *  - Provide a mechanism for adding routes with their associated handlers.
 *  - Clean up server resources before shutdown.
 *
 * Key Functions:
 *  - server_init()     : Allocates and initializes a new Server instance.
 *  - server_start()    : Starts the server to accept and handle client requests.
 *  - server_add_route(): Registers a route handler for a given HTTP method and path.
 *  - server_free()     : Releases resources allocated by the server.
 *
 * Typical Usage:
 *  @code
 *  Server *server = server_init(server; 8080, 10);
 *  if (!server) {
 *      fprintf(stderr, "Failed to initialize server\n");
 *      return 1;
 *  }
 *
 *  server_add_route(server, "GET", "/hello", hello_handler);
 *  server_start(server);
 *  
 *  // Free all heap allocated values
 *  server_free(server);
 *  @endcode
 *
 * @note All dynamically allocated resources must be released using server_free()
 *       to prevent memory leaks.
 *
 * @author  Karl-Alexandre Michaud
 * @date    2025-08-29
 */

#include "server.h"


/*
 * Initialize a new server instance.
 * - `port`: the server port number
 * - `max_clients`: the max number of concurrent clients
 * 
 * Returns pointer server struct if success, and NULL otherwise.
 *
 * NOTE: Call server_free() to shut down server and free allocated memory.
 */
Server *server_init(int port, int max_clients) {
    // Setup server struct
    Server *server = malloc(sizeof(struct Server));
    if (!server) {
        perror("malloc failed. Aborting server initialization.");
        return NULL;
    }

    server->port = port;
    server->max_clients = max_clients;
    
    memset(&server->addr, 0, sizeof(server->addr)); // Ensure safety by removing old data
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(port); // Necessary for big endian vs little endian
    server->addr.sin_addr.s_addr = INADDR_ANY;
        
    // Socket creation
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Protocol set by default to TCP
    if (sockfd == -1) {
        perror("socket creation failed. Aborting server initialization.");
        free(server);
        return NULL;
    }
    server->sockfd = sockfd;

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        server_free(server);
        return NULL;
    }

    // Bind socket to port
    int bind_res = bind(sockfd, (struct sockaddr*)&server->addr, sizeof(server->addr));
    if (bind_res == -1) {
        perror("socket binding failed. Aborting server initialization.");
        server_free(server);
        return NULL;
    }
    
    // Socket tracking set creation
    FD_ZERO(&server->sock_set);
    FD_SET(sockfd, &server->sock_set);
    server->max_fd = sockfd // max fd since it's the only fd (for now)

    return server;
}


/*
 * Free server resources. This, by extension, kills the server.
 * - `server`: pointer to Server struct
 */
int server_free(Server *server) {
    if (server->sockfd > 0) {
        close(server->sockfd);
    }
    free(server);
    return 1;
}

