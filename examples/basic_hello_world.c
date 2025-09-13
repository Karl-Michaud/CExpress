/**
 * @file basic_hello_world.c
 * @brief Basic example demonstrating a simple HTTP server with a single GET route.
 * 
 * This example shows the minimal setup required to create a CExpress server
 * with a single "Hello World" endpoint.
 * 
 * Compile: gcc -o basic_hello_world basic_hello_world.c ../src/*.c -I../include
 * Run: ./basic_hello_world
 * Test: curl http://localhost:8080/hello
 * 
 * @author Karl-Alexandre Michaud
 * @date 2025-01-27
 */

#include "../include/CExpress/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Handler function for the /hello route.
 * 
 * Returns a simple "Hello World" message.
 * 
 * @return Dynamically allocated string containing the response body.
 */
char *hello_handler(void) {
    char *response = malloc(50);
    if (!response) {
        perror("malloc failed in hello_handler");
        return NULL;
    }
    
    strcpy(response, "Hello, World from CExpress!");
    return response;
}

/**
 * @brief Handler function for the /time route.
 * 
 * Returns the current time as a string.
 * 
 * @return Dynamically allocated string containing the current time.
 */
char *time_handler(void) {
    char *response = malloc(100);
    if (!response) {
        perror("malloc failed in time_handler");
        return NULL;
    }
    
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(response, 100, "Current time: %Y-%m-%d %H:%M:%S", tm_info);
    
    return response;
}

int main(void) {
    printf("Starting CExpress Basic Hello World Server...\n");
    
    // Initialize server on port 8080, max 10 clients, backlog 5, DEV mode
    Server *server = server_init(8080, 10, 5, DEV);
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }
    
    printf("Server initialized successfully\n");
    printf("Listening on http://localhost:8080\n");
    printf("Press Ctrl+C to stop the server\n\n");
    
    // Add routes
    if (!server_add_route(server, GET, "/hello", hello_handler)) {
        fprintf(stderr, "Failed to add /hello route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/time", time_handler)) {
        fprintf(stderr, "Failed to add /time route\n");
        server_free(server);
        return 1;
    }
    
    printf("Routes registered:\n");
    printf("  GET /hello - Returns a hello message\n");
    printf("  GET /time  - Returns current time\n\n");
    
    // Start the server (this blocks until Ctrl+C)
    int result = server_start(server);
    
    if (result == -1) {
        fprintf(stderr, "Server failed to start\n");
        return 1;
    }
    
    printf("Server stopped gracefully\n");
    return 0;
}
