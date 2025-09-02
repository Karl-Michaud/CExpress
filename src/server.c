/**
 * @file server.c
 * @brief Core server implementation for the CExpress framework.
 *
 * This file provides the core logic for initializing and running a lightweight 
 * HTTP-like server in C. It defines and manages the Server structure, handling 
 * socket creation, binding, listening for incoming client connections, and 
 * routing requests to appropriate handlers based on the registered routes.
 *
 * **Responsibilities:**
 *  - Configure and initialize the server socket.
 *  - Start listening for incoming TCP connections on a specified port.
 *  - Dispatch client requests to the appropriate route handlers.
 *  - Provide utilities for adding and managing routes dynamically.
 *  - Clean up all allocated resources during shutdown.
 *
 * **Key Functions:**
 *  - `server_init()`      : Creates and configures a new Server instance.
 *  - `server_add_route()` : Registers a route with a handler for a specific HTTP method.
 *  - `server_start()`     : Begins accepting and handling client connections.
 *  - `server_free()`      : Frees all resources associated with the server.
 *
 * **Example Usage:**
 *  @code
 *  Server *server = server_init(8080, 10);
 *  if (!server) {
 *      fprintf(stderr, "Server initialization failed\n");
 *      return 1;
 *  }
 *
 *  server_add_route(server, "GET", "/hello", hello_handler);
 *  server_start(server);
 *
 *  server_free(server);
 *  @endcode
 *
 * @note Always call `server_free()` after the server loop ends to avoid memory leaks.
 *
 * @author  Karl-Alexandre Michaud
 * @date    2025-08-29
 */


#include "server.h"


volatile sig_atomic_t running = 0; // `volatile` prevents compiler optimizations that assume the value never changes unexpectedly.  
                                   // `sig_atomic_t` guarantees atomic read/write in a signal handler (designed for sig handlers).


/**
 * @brief Signal handler for SIGINT (Ctrl+C).
 *
 * This function is triggered when the server receives a SIGINT signal.
 * It sets the global `running` flag to 0, which signals the main loop
 * in `server_start()` to exit gracefully.
 *
 * @param signum The signal number (unused).
 *
 * @note Uses `sig_atomic_t` to ensure atomic operations inside signal handler.
 */
void handler_sigint(int signum) {
    running = 0; // Safe since type `sig_atomic_t`is designed for signal handlers
}


/**
 * @brief Removes a client from the server's client list.
 *
 * Closes the client's socket, clears the client structure, 
 * and updates the active client count.
 *
 * @param server Pointer to the Server instance.
 * @param index  Index of the client in the client list to remove.
 */
void remove_client(Server *server, int index) {
    if (index < 0 || index >= server->max_clients) {
        return;
    }
    
    close(server->client_lst[index].client_sock);         // close socket
    free(server->client_lst[index].router_lst.items);
    server->client_lst[index].router_lst.items = NULL;
    memset(&server->client_lst[index], 0, sizeof(client_t)); // zero out client struct
}


/**
 * @brief Initializes a new Server instance.
 *
 * Allocates and configures a server structure, creates the TCP socket,
 * sets socket options (SO_REUSEADDR), and binds the socket to the given port.
 *
 * @param port        The TCP port number for the server to listen on.
 * @param max_clients Maximum number of concurrent clients supported.
 * @param backlog     Maximum number of queued connection requests.
 *
 * @return Pointer to a dynamically allocated Server structure on success, 
 *         or NULL on failure.
 *
 * @note Caller is responsible for freeing resources using `server_free()`.
 */
Server *server_init(int port, int max_clients, int backlog) {
    // setup server struct
    Server *server = malloc(sizeof(Server));
    if (!server) {
        perror("malloc failed. aborting server initialization.");
        return NULL;
    }

    server->port = port;
    server->max_clients = max_clients;
    server->backlog = backlog;
    server->sockfd = -1; // Will be changed later if socket creation successful. Set to -1 for safety.

    server->client_lst = malloc(sizeof(client_t) * max_clients);
    if (!server->client_lst) {
        perror("malloc failed. aborting server initialization.");
        free(server);
        return NULL;
    }
    for (int i = 0; i < max_clients; i++) {
        memset(&server->client_lst[i], 0, sizeof(client_t)); // ensure safety by removing old data
        server->client_lst[i].client_sock = 0;

        // Init router list for each client
        server->client_lst[i].router_lst.count = 0;
        server->client_lst[i].router_lst.capacity = 4;
        server->client_lst[i].router_lst.items = malloc(server->client_lst[i].router_lst.capacity * sizeof(Router));
        if (!server->client_lst[i].router_lst.items) {
            perror("malloc failed. aborting server initialization.");
            // Free previously allocated router lists
            for (int j = 0; j < i; j++) {
                free(server->client_lst[j].router_lst.items);
                server->client_lst[j].router_lst.items = NULL;
            }
            free(server->client_lst);
            free(server);
            return NULL;
        }
        memset(server->client_lst[i].router_lst.items, 0, server->client_lst[i].router_lst.capacity * sizeof(Router));
    }
        
    memset(&server->addr, 0, sizeof(server->addr));
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(port); // Necessary for big endian vs little endian
    server->addr.sin_addr.s_addr = INADDR_ANY;
        
    // Socket creation
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Protocol set by default to TCP
    if (sockfd == -1) {
        perror("socket creation failed. Aborting server initialization.");
        server_free(server);
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
    
    return server;
}


/**
 * @brief Frees all resources associated with the server.
 *
 * Closes the server socket (if open) and releases all dynamically allocated memory.
 *
 * @param server Pointer to the Server instance to free.
 *
 */
void server_free(Server *server) {
    if (!server) return;

    if (server->sockfd != -1) {
        close(server->sockfd);
    }
    for (int j = 0; j < server->max_clients; j++) {
        free(server->client_lst[j].router_lst.items);
        server->client_lst[j].router_lst.items = NULL;
    }
    free(server->client_lst);
    free(server);
}


/**
 * @brief Starts the server's main loop to accept and handle client connections.
 *
 * This function:
 *  - Listens for incoming TCP connections using `listen()`.
 *  - Handles `SIGINT` for graceful shutdown.
 *  - Uses `select()` to monitor multiple client sockets for incoming data.
 *  - Accepts new clients and tracks them in the client list.
 *  - Removes clients on disconnection or read errors.
 *
 * @param server Pointer to the Server instance.
 *
 * @return 1 on successful shutdown, -1 if an error occurred during setup.
 *
 * @note This function runs an infinite loop until interrupted by SIGINT.
 */
int server_start(Server *server) {
    fd_set sock_set;     // all client/server sockets being monitored by server (used for select() sys call)
    int max_fd, num_clients = 0;         

    // Listen for connections
    if (listen(server->sockfd, server->backlog) < 0) {
        perror("listen failed. Aborting server start.");
        // server_free(server);
        return -1;
    }

    // Setup signal handling
    struct sigaction newact;
    newact.sa_handler = handler_sigint;
    newact.sa_flags = 0;
    sigemptyset(&newact.sa_mask);

    if (sigaction(SIGINT, &newact, NULL) == -1) {
        perror("sigaction failed. Aborting server start.");
        // server_free(server);
        return -1;
    }

    // Set signal var
    running = 1;

    while (running) {
        // Socket tracking set creation
        FD_ZERO(&sock_set);
        FD_SET(server->sockfd, &sock_set);
        max_fd = server->sockfd; // max fd since it's the only fd (for now)
        
        // Add client fds to set
        for (int i = 0; i < server->max_clients; i++) {
            if (server->client_lst[i].client_sock > 0) {
                FD_SET(server->client_lst[i].client_sock, &sock_set);
            }
            if (server->client_lst[i].client_sock > max_fd) {
                max_fd = server->client_lst[i].client_sock;
            }
        }

        // Check for activity
        if (select(max_fd + 1, &sock_set, NULL, NULL, NULL)< 0) {
            perror("selection failed. Skipping.");
            continue; // skip iteration
        }

        // If server socket is flagged, then a client is attempting to connect
        if (FD_ISSET(server->sockfd, &sock_set)) {
            struct sockaddr_in client_addr;
            socklen_t size_struct = sizeof(client_addr);

            int new_socket = accept(server->sockfd, (struct sockaddr *)&client_addr, &size_struct);
            if (new_socket < 0) {
                perror("accept failed. Skipping.");
                continue;
            }

            for (int i = 0; i< server->max_clients; i++) {
                if (num_clients >= server->max_clients) {
                    break;
                }
                if (server->client_lst[i].client_sock == 0) {
                    num_clients++;
                    server->client_lst[i].client_sock = new_socket;
                    server->client_lst[i].addr = client_addr;
                    break;
                }
            }
        }

        for (int i = 0; i < server->max_clients; i++) {
           if (FD_ISSET(server->client_lst[i].client_sock, &sock_set)) {
                char buffer[BUFFER_SIZE];
                int chars_read = read(server->client_lst[i].client_sock, buffer, sizeof(buffer) - 1);
                if (chars_read == 0) {
                    // Client has been disconnected. Remove from client list.
                    remove_client(server, i);
                    num_clients--;
                } else if (chars_read < 0) {
                    if (errno == EINTR) {
                        // Interrupted by a signal, safe to retry. Read will be tried again.
                        continue;
                    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // Non-blocking socket. no data yet, so handle later.
                        break;
                    } else {
                        // Other errors: disconnect client
                        perror("read failed. Skipping");
                        remove_client(server, i);
                        num_clients--;
                        break;
                    }

                } else {
                    // Read was successful. process data!
                    if (!process_header(buffer, server->client_lst[i].client_sock, &server->client_lst[i].router_lst)) {
                        // Route not found or handler failed
                        const char *not_found = "HTTP/1.1 404 Not Found\r\n"
                                                "Content-Length: 0\r\n"
                                                "Connection: close\r\n\r\n";
                        write(server->client_lst[i].client_sock, not_found, strlen(not_found));

                        remove_client(server, i);
                    }
                    continue;
                }
           }
        }
    }

    // Cleanup when server stops
    for (size_t i = 0; i < server->max_clients; i++) {
        if (server->client_lst[i].client_sock > 0) {
            remove_client(server, i);
        }    
    }
    server_free(server);
    return 1;
}


/**
 * @brief Adds a new route to the client's router list.
 *
 * Creates a Router struct from the provided method, path, and handler,
 * and adds it to the dynamic RouterList. Expands the list capacity if needed.
 *
 * @param routers Pointer to the RouterList where the route will be added.
 * @param method  The HTTP method (GET, POST, PUT, DELETE) for this route.
 * @param path    The path (URL) that this route should match.
 * @param handler The function to handle requests matching this method and path.
 *
 * @return 1 on success, 0 if adding the route fails (e.g., memory allocation failure).
 */
int client_add_route(RouterList *routers, method_t method, path_t path, HandlerFunc handler) {
    Router new_router;
    new_router.method = method;
    new_router.path = path;
    new_router.handler = handler;

    return add_route(routers, new_router);
}


/**
 * @brief Removes a route from the client's router list by its index.
 *
 * Retrieves the Router at the given index and removes it from the RouterList.
 * The route list is compacted to fill the removed slot.
 *
 * @param router_lst Pointer to the RouterList.
 * @param index      Index of the route to remove (0-based).
 *
 * @return 1 if the route was successfully removed, 0 if the index is invalid.
 */
int client_remove_route(RouterList *router_lst, size_t index) {
    if (index >= router_lst->count) {
        return 0;
    }

    Router router_remove = router_lst->items[index];
    return remove_route(router_lst, router_remove);
}

