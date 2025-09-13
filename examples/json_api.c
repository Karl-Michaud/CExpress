/**
 * @file json_api.c
 * @brief Example demonstrating a JSON API with multiple HTTP methods and routes.
 * 
 * This example shows how to create a more complex API that returns JSON responses
 * and handles different HTTP methods (GET, POST, PUT, DELETE).
 * 
 * Compile: gcc -o json_api json_api.c ../src/*.c -I../include
 * Run: ./json_api
 * 
 * Test endpoints:
 *   curl http://localhost:8080/api/users
 *   curl http://localhost:8080/api/status
 *   curl -X POST http://localhost:8080/api/users -d '{"name":"John","email":"john@example.com"}'
 *   curl -X PUT http://localhost:8080/api/users/1 -d '{"name":"Jane","email":"jane@example.com"}'
 *   curl -X DELETE http://localhost:8080/api/users/1
 * 
 * @author Karl-Alexandre Michaud
 * @date 2025-01-27
 */

#include "../include/CExpress/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Simple in-memory user storage
typedef struct {
    int id;
    char name[50];
    char email[100];
} User;

User users[10];
int user_count = 0;
int next_id = 1;

/**
 * @brief Handler for GET /api/users - Returns list of all users
 */
char *get_users_handler(void) {
    char *response = malloc(1024);
    if (!response) {
        perror("malloc failed in get_users_handler");
        return NULL;
    }
    
    strcpy(response, "{\n  \"users\": [\n");
    
    for (int i = 0; i < user_count; i++) {
        char user_json[200];
        snprintf(user_json, sizeof(user_json), 
                "    {\n      \"id\": %d,\n      \"name\": \"%s\",\n      \"email\": \"%s\"\n    }%s\n",
                users[i].id, users[i].name, users[i].email,
                (i < user_count - 1) ? "," : "");
        
        strcat(response, user_json);
    }
    
    strcat(response, "  ]\n}");
    return response;
}

/**
 * @brief Handler for GET /api/status - Returns server status
 */
char *get_status_handler(void) {
    char *response = malloc(200);
    if (!response) {
        perror("malloc failed in get_status_handler");
        return NULL;
    }
    
    time_t now = time(NULL);
    snprintf(response, 200, 
            "{\n  \"status\": \"running\",\n  \"timestamp\": %ld,\n  \"users_count\": %d\n}",
            now, user_count);
    
    return response;
}

/**
 * @brief Handler for POST /api/users - Creates a new user
 */
char *post_users_handler(void) {
    char *response = malloc(200);
    if (!response) {
        perror("malloc failed in post_users_handler");
        return NULL;
    }
    
    if (user_count >= 10) {
        strcpy(response, "{\n  \"error\": \"Maximum number of users reached\"\n}");
        return response;
    }
    
    // In a real application, you would parse the JSON body here
    // For this example, we'll create a dummy user
    users[user_count].id = next_id++;
    strcpy(users[user_count].name, "New User");
    strcpy(users[user_count].email, "newuser@example.com");
    user_count++;
    
    snprintf(response, 200, 
            "{\n  \"message\": \"User created successfully\",\n  \"id\": %d\n}",
            users[user_count - 1].id);
    
    return response;
}

/**
 * @brief Handler for PUT /api/users - Updates a user
 */
char *put_users_handler(void) {
    char *response = malloc(200);
    if (!response) {
        perror("malloc failed in put_users_handler");
        return NULL;
    }
    
    // In a real application, you would parse the JSON body and user ID
    // For this example, we'll update the first user
    if (user_count > 0) {
        strcpy(users[0].name, "Updated User");
        strcpy(users[0].email, "updated@example.com");
        
        snprintf(response, 200, 
                "{\n  \"message\": \"User updated successfully\",\n  \"id\": %d\n}",
                users[0].id);
    } else {
        strcpy(response, "{\n  \"error\": \"No users to update\"\n}");
    }
    
    return response;
}

/**
 * @brief Handler for DELETE /api/users - Deletes a user
 */
char *delete_users_handler(void) {
    char *response = malloc(200);
    if (!response) {
        perror("malloc failed in delete_users_handler");
        return NULL;
    }
    
    if (user_count > 0) {
        int deleted_id = users[0].id;
        
        // Shift users left
        for (int i = 0; i < user_count - 1; i++) {
            users[i] = users[i + 1];
        }
        user_count--;
        
        snprintf(response, 200, 
                "{\n  \"message\": \"User deleted successfully\",\n  \"id\": %d\n}",
                deleted_id);
    } else {
        strcpy(response, "{\n  \"error\": \"No users to delete\"\n}");
    }
    
    return response;
}

/**
 * @brief Handler for GET /api/health - Health check endpoint
 */
char *get_health_handler(void) {
    char *response = malloc(100);
    if (!response) {
        perror("malloc failed in get_health_handler");
        return NULL;
    }
    
    strcpy(response, "{\n  \"status\": \"healthy\",\n  \"uptime\": \"running\"\n}");
    return response;
}

int main(void) {
    printf("Starting CExpress JSON API Server...\n");
    
    // Initialize server on port 8080, max 20 clients, backlog 10, DEV mode
    Server *server = server_init(8080, 20, 10, DEV);
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }
    
    printf("Server initialized successfully\n");
    printf("Listening on http://localhost:8080\n");
    printf("Press Ctrl+C to stop the server\n\n");
    
    // Add API routes
    if (!server_add_route(server, GET, "/api/users", get_users_handler)) {
        fprintf(stderr, "Failed to add GET /api/users route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/api/status", get_status_handler)) {
        fprintf(stderr, "Failed to add GET /api/status route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/api/health", get_health_handler)) {
        fprintf(stderr, "Failed to add GET /api/health route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, POST, "/api/users", post_users_handler)) {
        fprintf(stderr, "Failed to add POST /api/users route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, PUT, "/api/users", put_users_handler)) {
        fprintf(stderr, "Failed to add PUT /api/users route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, DELETE, "/api/users", delete_users_handler)) {
        fprintf(stderr, "Failed to add DELETE /api/users route\n");
        server_free(server);
        return 1;
    }
    
    printf("API Routes registered:\n");
    printf("  GET    /api/users   - List all users\n");
    printf("  GET    /api/status  - Server status\n");
    printf("  GET    /api/health  - Health check\n");
    printf("  POST   /api/users   - Create new user\n");
    printf("  PUT    /api/users   - Update user\n");
    printf("  DELETE /api/users   - Delete user\n\n");
    
    printf("Example curl commands:\n");
    printf("  curl http://localhost:8080/api/users\n");
    printf("  curl http://localhost:8080/api/status\n");
    printf("  curl -X POST http://localhost:8080/api/users\n");
    printf("  curl -X PUT http://localhost:8080/api/users\n");
    printf("  curl -X DELETE http://localhost:8080/api/users\n\n");
    
    // Start the server (this blocks until Ctrl+C)
    int result = server_start(server);
    
    if (result == -1) {
        fprintf(stderr, "Server failed to start\n");
        return 1;
    }
    
    printf("Server stopped gracefully\n");
    return 0;
}
