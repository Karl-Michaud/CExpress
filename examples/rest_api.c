/**
 * @file rest_api.c
 * @brief Example demonstrating a full REST API with CRUD operations.
 * 
 * This example shows how to build a complete REST API with proper HTTP methods,
 * status codes, and JSON responses. It includes a simple in-memory database
 * for managing products with full CRUD operations.
 * 
 * Compile: gcc -o rest_api rest_api.c ../src/*.c -I../include
 * Run: ./rest_api
 * 
 * API Endpoints:
 *   GET    /api/products     - List all products
 *   GET    /api/products/1   - Get specific product
 *   POST   /api/products     - Create new product
 *   PUT    /api/products/1   - Update product
 *   DELETE /api/products/1   - Delete product
 *   GET    /api/products/search?name=laptop - Search products
 * 
 * Example requests:
 *   curl http://localhost:8080/api/products
 *   curl -X POST http://localhost:8080/api/products -d '{"name":"Laptop","price":999.99,"category":"Electronics"}'
 *   curl -X PUT http://localhost:8080/api/products/1 -d '{"name":"Gaming Laptop","price":1299.99}'
 *   curl -X DELETE http://localhost:8080/api/products/1
 * 
 * @author Karl-Alexandre Michaud
 * @date 2025-01-27
 */

#include <CExpress/server.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Product structure
typedef struct {
    int id;
    char name[100];
    double price;
    char category[50];
    char description[200];
    time_t created_at;
    time_t updated_at;
} Product;

// Simple in-memory database
Product products[50];
int product_count = 0;
int next_id = 1;

/**
 * @brief Helper function to find a product by ID
 */
Product* find_product_by_id(int id) {
    for (int i = 0; i < product_count; i++) {
        if (products[i].id == id) {
            return &products[i];
        }
    }
    return NULL;
}

/**
 * @brief Helper function to create a JSON error response
 */
char* create_error_response(const char* message, int status_code) {
    char *response = malloc(200);
    if (!response) {
        perror("malloc failed in create_error_response");
        return NULL;
    }
    
    snprintf(response, 200, 
            "{\n  \"error\": \"%s\",\n  \"status\": %d,\n  \"timestamp\": %ld\n}",
            message, status_code, time(NULL));
    
    return response;
}

/**
 * @brief Helper function to create a JSON success response
 */
char* create_success_response(const char* message, int status_code) {
    char *response = malloc(200);
    if (!response) {
        perror("malloc failed in create_success_response");
        return NULL;
    }
    
    snprintf(response, 200, 
            "{\n  \"message\": \"%s\",\n  \"status\": %d,\n  \"timestamp\": %ld\n}",
            message, status_code, time(NULL));
    
    return response;
}

/**
 * @brief GET /api/products - List all products
 */
char *get_products_handler(void) {
    char *response = malloc(2048);
    if (!response) {
        perror("malloc failed in get_products_handler");
        return NULL;
    }
    
    strcpy(response, "{\n  \"products\": [\n");
    
    for (int i = 0; i < product_count; i++) {
        char product_json[500];
        snprintf(product_json, sizeof(product_json), 
                "    {\n"
                "      \"id\": %d,\n"
                "      \"name\": \"%s\",\n"
                "      \"price\": %.2f,\n"
                "      \"category\": \"%s\",\n"
                "      \"description\": \"%s\",\n"
                "      \"created_at\": %ld,\n"
                "      \"updated_at\": %ld\n"
                "    }%s\n",
                products[i].id, products[i].name, products[i].price,
                products[i].category, products[i].description,
                products[i].created_at, products[i].updated_at,
                (i < product_count - 1) ? "," : "");
        
        strcat(response, product_json);
    }
    
    strcat(response, "  ],\n  \"count\": ");
    char count_str[10];
    snprintf(count_str, sizeof(count_str), "%d", product_count);
    strcat(response, count_str);
    strcat(response, "\n}");
    
    return response;
}

/**
 * @brief GET /api/products/{id} - Get specific product
 */
char *get_product_handler(void) {
    // In a real implementation, you would extract the ID from the URL
    // For this example, we'll get the first product
    if (product_count == 0) {
        return create_error_response("No products found", 404);
    }
    
    Product *product = &products[0];
    char *response = malloc(500);
    if (!response) {
        perror("malloc failed in get_product_handler");
        return NULL;
    }
    
    snprintf(response, 500,
            "{\n"
            "  \"product\": {\n"
            "    \"id\": %d,\n"
            "    \"name\": \"%s\",\n"
            "    \"price\": %.2f,\n"
            "    \"category\": \"%s\",\n"
            "    \"description\": \"%s\",\n"
            "    \"created_at\": %ld,\n"
            "    \"updated_at\": %ld\n"
            "  }\n"
            "}",
            product->id, product->name, product->price,
            product->category, product->description,
            product->created_at, product->updated_at);
    
    return response;
}

/**
 * @brief POST /api/products - Create new product
 */
char *create_product_handler(void) {
    if (product_count >= 50) {
        return create_error_response("Maximum number of products reached", 400);
    }
    
    // In a real implementation, you would parse the JSON body
    // For this example, we'll create a sample product
    Product *new_product = &products[product_count];
    new_product->id = next_id++;
    strcpy(new_product->name, "New Product");
    strcpy(new_product->category, "General");
    strcpy(new_product->description, "A new product created via API");
    new_product->price = 99.99;
    new_product->created_at = time(NULL);
    new_product->updated_at = new_product->created_at;
    
    product_count++;
    
    char *response = malloc(300);
    if (!response) {
        perror("malloc failed in create_product_handler");
        return NULL;
    }
    
    snprintf(response, 300,
            "{\n"
            "  \"message\": \"Product created successfully\",\n"
            "  \"product\": {\n"
            "    \"id\": %d,\n"
            "    \"name\": \"%s\",\n"
            "    \"price\": %.2f\n"
            "  }\n"
            "}",
            new_product->id, new_product->name, new_product->price);
    
    return response;
}

/**
 * @brief PUT /api/products/{id} - Update product
 */
char *update_product_handler(void) {
    if (product_count == 0) {
        return create_error_response("No products to update", 404);
    }
    
    // In a real implementation, you would parse the JSON body and extract ID
    // For this example, we'll update the first product
    Product *product = &products[0];
    strcpy(product->name, "Updated Product");
    strcpy(product->description, "This product has been updated via API");
    product->price = 149.99;
    product->updated_at = time(NULL);
    
    char *response = malloc(300);
    if (!response) {
        perror("malloc failed in update_product_handler");
        return NULL;
    }
    
    snprintf(response, 300,
            "{\n"
            "  \"message\": \"Product updated successfully\",\n"
            "  \"product\": {\n"
            "    \"id\": %d,\n"
            "    \"name\": \"%s\",\n"
            "    \"price\": %.2f\n"
            "  }\n"
            "}",
            product->id, product->name, product->price);
    
    return response;
}

/**
 * @brief DELETE /api/products/{id} - Delete product
 */
char *delete_product_handler(void) {
    if (product_count == 0) {
        return create_error_response("No products to delete", 404);
    }
    
    int deleted_id = products[0].id;
    
    // Shift products left
    for (int i = 0; i < product_count - 1; i++) {
        products[i] = products[i + 1];
    }
    product_count--;
    
    char *response = malloc(200);
    if (!response) {
        perror("malloc failed in delete_product_handler");
        return NULL;
    }
    
    snprintf(response, 200,
            "{\n"
            "  \"message\": \"Product deleted successfully\",\n"
            "  \"deleted_id\": %d\n"
            "}",
            deleted_id);
    
    return response;
}

/**
 * @brief GET /api/products/search - Search products
 */
char *search_products_handler(void) {
    char *response = malloc(1024);
    if (!response) {
        perror("malloc failed in search_products_handler");
        return NULL;
    }
    
    // In a real implementation, you would parse query parameters
    // For this example, we'll return all products
    strcpy(response, "{\n  \"search_results\": [\n");
    
    for (int i = 0; i < product_count; i++) {
        char product_json[300];
        snprintf(product_json, sizeof(product_json), 
                "    {\n"
                "      \"id\": %d,\n"
                "      \"name\": \"%s\",\n"
                "      \"price\": %.2f,\n"
                "      \"category\": \"%s\"\n"
                "    }%s\n",
                products[i].id, products[i].name, products[i].price,
                products[i].category,
                (i < product_count - 1) ? "," : "");
        
        strcat(response, product_json);
    }
    
    strcat(response, "  ],\n  \"total_found\": ");
    char count_str[10];
    snprintf(count_str, sizeof(count_str), "%d", product_count);
    strcat(response, count_str);
    strcat(response, "\n}");
    
    return response;
}

/**
 * @brief GET /api/stats - Get API statistics
 */
char *get_stats_handler(void) {
    char *response = malloc(300);
    if (!response) {
        perror("malloc failed in get_stats_handler");
        return NULL;
    }
    
    snprintf(response, 300,
            "{\n"
            "  \"api_name\": \"CExpress REST API\",\n"
            "  \"version\": \"1.0.0\",\n"
            "  \"total_products\": %d,\n"
            "  \"server_time\": %ld,\n"
            "  \"endpoints\": 6\n"
            "}",
            product_count, time(NULL));
    
    return response;
}

int main(void) {
    printf("Starting CExpress REST API Server...\n");
    
    // Initialize some sample products
    Product sample1 = {1, "Gaming Laptop", 1299.99, "Electronics", "High-performance gaming laptop", time(NULL), time(NULL)};
    Product sample2 = {2, "Wireless Mouse", 29.99, "Accessories", "Ergonomic wireless mouse", time(NULL), time(NULL)};
    Product sample3 = {3, "Mechanical Keyboard", 89.99, "Accessories", "RGB mechanical keyboard", time(NULL), time(NULL)};
    
    products[0] = sample1;
    products[1] = sample2;
    products[2] = sample3;
    product_count = 3;
    next_id = 4;
    
    // Initialize server on port 8080, max 25 clients, backlog 10, DEV mode
    Server *server = server_init(8080, 25, 10, DEV);
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }
    
    printf("Server initialized successfully\n");
    printf("Listening on http://localhost:8080\n");
    printf("Press Ctrl+C to stop the server\n\n");
    
    // Add REST API routes
    if (!server_add_route(server, GET, "/api/products", get_products_handler)) {
        fprintf(stderr, "Failed to add GET /api/products route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/api/products/1", get_product_handler)) {
        fprintf(stderr, "Failed to add GET /api/products/1 route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, POST, "/api/products", create_product_handler)) {
        fprintf(stderr, "Failed to add POST /api/products route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, PUT, "/api/products/1", update_product_handler)) {
        fprintf(stderr, "Failed to add PUT /api/products/1 route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, DELETE, "/api/products/1", delete_product_handler)) {
        fprintf(stderr, "Failed to add DELETE /api/products/1 route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/api/products/search", search_products_handler)) {
        fprintf(stderr, "Failed to add GET /api/products/search route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/api/stats", get_stats_handler)) {
        fprintf(stderr, "Failed to add GET /api/stats route\n");
        server_free(server);
        return 1;
    }
    
    printf("REST API Routes registered:\n");
    printf("  GET    /api/products        - List all products\n");
    printf("  GET    /api/products/1      - Get specific product\n");
    printf("  POST   /api/products        - Create new product\n");
    printf("  PUT    /api/products/1      - Update product\n");
    printf("  DELETE /api/products/1      - Delete product\n");
    printf("  GET    /api/products/search - Search products\n");
    printf("  GET    /api/stats           - API statistics\n\n");
    
    printf("Sample products loaded: %d\n", product_count);
    printf("Example curl commands:\n");
    printf("  curl http://localhost:8080/api/products\n");
    printf("  curl http://localhost:8080/api/stats\n");
    printf("  curl -X POST http://localhost:8080/api/products\n");
    printf("  curl -X PUT http://localhost:8080/api/products/1\n");
    printf("  curl -X DELETE http://localhost:8080/api/products/1\n\n");
    
    // Start the server (this blocks until Ctrl+C)
    int result = server_start(server);
    
    if (result == -1) {
        fprintf(stderr, "Server failed to start\n");
        return 1;
    }
    
    printf("Server stopped gracefully\n");
    return 0;
}
