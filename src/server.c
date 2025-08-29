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


volatile sig_atomic_t running = 0; // `volatile` prevents compiler optimizations that assume the value never changes unexpectedly.  
                                   // `sig_atomic_t` guarantees atomic read/write in a signal handler (designed for sig handlers).


void handler_sigint(int signum) {
    running = 0; // Safe since type `sig_atomic_t`is designed for signal handlers
}


/*
 * Initialize a new server instance.
 * - `port`: the server port number
 * - `max_clients`: the max number of concurrent clients
 * - `backlog`: numberof partially completed connections (queue)
 * 
 * Returns pointer server struct if success, and NULL otherwise.
 *
 * NOTE: Call server_free() to shut down server and free allocated memory.
 */
Server *server_init(int port, int max_clients, int backlog) {
    // Setup server struct
    Server *server = malloc(sizeof(struct Server));
    if (!server) {
        perror("malloc failed. Aborting server initialization.");
        return NULL;
    }

    server->port = port;
    server->max_clients = max_clients;
    server->backlog = backlog;
    
    int len_lst = sizeof(client_t) * max_clients
    server->client_lst = malloc(len_lst);
    if (!server->client_lst) {
        perror("malloc failed. Aborting server initialization.");
        free(server);
        return NULL
    }
    for (int i = 0; i < len_lst; i++) {
        memset(&server->client_lst[i], 0, sizeof(server->client_lst[i])); // Ensure safety by removing old data
        server->client_lst[i].client_sock = 0;
    }
    
    memset(&server->addr, 0, sizeof(server->addr)); // Ensure safety by removing old data
    server->addr.sin_family = AF_INET;
    server->addr.sin_port = htons(port); // Necessary for big endian vs little endian
    server->addr.sin_addr.s_addr = INADDR_ANY;
        
    // Socket creation
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Protocol set by default to TCP
    if (sockfd == -1) {
        perror("socket creation failed. Aborting server initialization.");
        free(server->client_lst);
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
    free(server->client_lst);
    free(server);
    return 1;
}


/*
 * Start listening and handling requests.
 * - `server`: pointer to Server stuct
 */
int server_start(Server *server) {
    fd_set sock_set;     // all client/server sockets being monitored by server (used for select() sys call)
    int max_fd, num_clients;         

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
        FD_SET(sockfd, &sock_set);
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
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL)< 0) {
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
                    server->client_lst[i].client_sock = new_sock;
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
                    remove_client(server, i, &num_clients);
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
                        remove_client(server, i, &num_clients);
                        break;
                    }

                } else {
                    // Read was successfull
                    // process data!
                    continue;
                }
           }
        }
    }
}


/*
 * Remove client at given index from client list.
 * - `server`: pointer to instance of server
 * - `index`: index of the client that needs to be removed
 * - `count`: pointer to client tracker
 */
void remove_client(Server *server, int index, int *count) {
    if (index < 0 || index >= server->client_count) {
        return;
    }

    close(server->client_lst[index].client_sock);         // close socket
    memset(&server->client_lst[index], 0, sizeof(client_t)); // zero out client struct
    *count = *count - 1;
}

