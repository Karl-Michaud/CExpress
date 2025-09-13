# Usage Examples

## Quick Navigation

- [Basic Hello World](#basic-hello-world)
- [JSON API](#json-api)
- [Static Files](#static-files)
- [REST API](#rest-api)

## Basic Hello World

```c
#include "include/CExpress/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *hello_handler(void) {
    char *response = malloc(50);
    strcpy(response, "Hello, World from CExpress!");
    return response;
}

char *time_handler(void) {
    char *response = malloc(100);
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(response, 100, "Current time: %Y-%m-%d %H:%M:%S", tm_info);
    return response;
}

int main(void) {
    printf("Starting CExpress Basic Hello World Server...\n");
    
    Server *server = server_init(8080, 10, 5, DEV);
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }
    
    server_add_route(server, GET, "/hello", hello_handler);
    server_add_route(server, GET, "/time", time_handler);
    
    printf("Server running on http://localhost:8080\n");
    server_start(server);
    return 0;
}
```

## JSON API

```c
#include "include/CExpress/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char name[50];
    char email[100];
} User;

User users[10];
int user_count = 0;
int next_id = 1;

char *get_users_handler(void) {
    char *response = malloc(1024);
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

char *post_users_handler(void) {
    char *response = malloc(200);
    if (user_count >= 10) {
        strcpy(response, "{\n  \"error\": \"Maximum number of users reached\"\n}");
        return response;
    }
    
    users[user_count].id = next_id++;
    strcpy(users[user_count].name, "New User");
    strcpy(users[user_count].email, "newuser@example.com");
    user_count++;
    
    snprintf(response, 200, 
            "{\n  \"message\": \"User created successfully\",\n  \"id\": %d\n}",
            users[user_count - 1].id);
    
    return response;
}

int main(void) {
    printf("Starting CExpress JSON API Server...\n");
    
    Server *server = server_init(8080, 20, 10, DEV);
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }
    
    server_add_route(server, GET, "/api/users", get_users_handler);
    server_add_route(server, POST, "/api/users", post_users_handler);
    
    printf("API running on http://localhost:8080\n");
    server_start(server);
    return 0;
}
```

## Static Files

```c
#include "include/CExpress/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *serve_index(void) {
    char *response = malloc(1024);
    strcpy(response, 
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <title>CExpress Static Server</title>\n"
        "    <link rel=\"stylesheet\" href=\"/static/style.css\">\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"container\">\n"
        "        <h1>Welcome to CExpress!</h1>\n"
        "        <p>This is a static file server example.</p>\n"
        "        <nav>\n"
        "            <a href=\"/\">Home</a>\n"
        "            <a href=\"/about\">About</a>\n"
        "        </nav>\n"
        "    </div>\n"
        "</body>\n"
        "</html>"
    );
    return response;
}

char *serve_css(void) {
    char *response = malloc(1024);
    strcpy(response,
        "body {\n"
        "    font-family: Arial, sans-serif;\n"
        "    margin: 0;\n"
        "    padding: 20px;\n"
        "    background-color: #f5f5f5;\n"
        "}\n"
        ".container {\n"
        "    max-width: 800px;\n"
        "    margin: 0 auto;\n"
        "    background: white;\n"
        "    padding: 30px;\n"
        "    border-radius: 8px;\n"
        "}\n"
        "h1 { color: #333; }\n"
        "nav a { margin-right: 20px; color: #007acc; }\n"
    );
    return response;
}

int main(void) {
    printf("Starting CExpress Static File Server...\n");
    
    Server *server = server_init(8080, 15, 5, DEV);
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }
    
    server_add_route(server, GET, "/", serve_index);
    server_add_route(server, GET, "/static/style.css", serve_css);
    
    printf("Server running on http://localhost:8080\n");
    server_start(server);
    return 0;
}
```

## REST API

```c
#include "include/CExpress/server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char name[100];
    double price;
    char category[50];
} Product;

Product products[50];
int product_count = 0;
int next_id = 1;

char *get_products_handler(void) {
    char *response = malloc(2048);
    strcpy(response, "{\n  \"products\": [\n");
    
    for (int i = 0; i < product_count; i++) {
        char product_json[500];
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
    
    strcat(response, "  ]\n}");
    return response;
}

char *create_product_handler(void) {
    char *response = malloc(200);
    if (product_count >= 50) {
        strcpy(response, "{\n  \"error\": \"Maximum products reached\"\n}");
        return response;
    }
    
    products[product_count].id = next_id++;
    strcpy(products[product_count].name, "New Product");
    strcpy(products[product_count].category, "General");
    products[product_count].price = 99.99;
    product_count++;
    
    snprintf(response, 200,
            "{\n"
            "  \"message\": \"Product created\",\n"
            "  \"id\": %d\n"
            "}",
            products[product_count - 1].id);
    
    return response;
}

int main(void) {
    printf("Starting CExpress REST API Server...\n");
    
    Server *server = server_init(8080, 25, 10, DEV);
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }
    
    server_add_route(server, GET, "/api/products", get_products_handler);
    server_add_route(server, POST, "/api/products", create_product_handler);
    
    printf("API running on http://localhost:8080\n");
    server_start(server);
    return 0;
}
```
