/**
 * @file static_files.c
 * @brief Example demonstrating how to serve static HTML files and assets.
 * 
 * This example shows how to create a web server that serves static files
 * like HTML, CSS, and JavaScript. It includes a simple file serving mechanism
 * and demonstrates different content types.
 * 
 * Compile: gcc -o static_files static_files.c ../src/*.c -I../include
 * Run: ./static_files
 * 
 * Test endpoints:
 *   curl http://localhost:8080/
 *   curl http://localhost:8080/about
 *   curl http://localhost:8080/api/info
 *   curl http://localhost:8080/static/style.css
 *   curl http://localhost:8080/static/script.js
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
 * @brief Serves the main HTML page
 */
char *serve_index(void) {
    char *response = malloc(1024);
    if (!response) {
        perror("malloc failed in serve_index");
        return NULL;
    }
    
    strcpy(response, 
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
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
        "            <a href=\"/api/info\">API Info</a>\n"
        "        </nav>\n"
        "        <div class=\"content\">\n"
        "            <h2>Features</h2>\n"
        "            <ul>\n"
        "                <li>Static file serving</li>\n"
        "                <li>HTML content</li>\n"
        "                <li>CSS styling</li>\n"
        "                <li>JavaScript support</li>\n"
        "            </ul>\n"
        "        </div>\n"
        "        <script src=\"/static/script.js\"></script>\n"
        "    </div>\n"
        "</body>\n"
        "</html>"
    );
    
    return response;
}

/**
 * @brief Serves the about page
 */
char *serve_about(void) {
    char *response = malloc(1024);
    if (!response) {
        perror("malloc failed in serve_about");
        return NULL;
    }
    
    strcpy(response,
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>About - CExpress</title>\n"
        "    <link rel=\"stylesheet\" href=\"/static/style.css\">\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"container\">\n"
        "        <h1>About CExpress</h1>\n"
        "        <nav>\n"
        "            <a href=\"/\">Home</a>\n"
        "            <a href=\"/about\">About</a>\n"
        "            <a href=\"/api/info\">API Info</a>\n"
        "        </nav>\n"
        "        <div class=\"content\">\n"
        "            <h2>What is CExpress?</h2>\n"
        "            <p>CExpress is a lightweight HTTP server framework written in C.</p>\n"
        "            <p>It provides:</p>\n"
        "            <ul>\n"
        "                <li>Simple routing system</li>\n"
        "                <li>Handler-based architecture</li>\n"
        "                <li>Static file serving capabilities</li>\n"
        "                <li>JSON API support</li>\n"
        "                <li>Cross-platform compatibility</li>\n"
        "            </ul>\n"
        "            <h2>Performance</h2>\n"
        "            <p>Built for speed and efficiency, CExpress is perfect for:</p>\n"
        "            <ul>\n"
        "                <li>Microservices</li>\n"
        "                <li>API endpoints</li>\n"
        "                <li>Static file hosting</li>\n"
        "                <li>Prototyping</li>\n"
        "            </ul>\n"
        "        </div>\n"
        "    </div>\n"
        "</body>\n"
        "</html>"
    );
    
    return response;
}

/**
 * @brief Serves CSS styles
 */
char *serve_css(void) {
    char *response = malloc(1024);
    if (!response) {
        perror("malloc failed in serve_css");
        return NULL;
    }
    
    strcpy(response,
        "/* CExpress Static Server Styles */\n"
        "body {\n"
        "    font-family: Arial, sans-serif;\n"
        "    margin: 0;\n"
        "    padding: 20px;\n"
        "    background-color: #f5f5f5;\n"
        "}\n"
        "\n"
        ".container {\n"
        "    max-width: 800px;\n"
        "    margin: 0 auto;\n"
        "    background: white;\n"
        "    padding: 30px;\n"
        "    border-radius: 8px;\n"
        "    box-shadow: 0 2px 10px rgba(0,0,0,0.1);\n"
        "}\n"
        "\n"
        "h1 {\n"
        "    color: #333;\n"
        "    border-bottom: 2px solid #007acc;\n"
        "    padding-bottom: 10px;\n"
        "}\n"
        "\n"
        "h2 {\n"
        "    color: #555;\n"
        "    margin-top: 30px;\n"
        "}\n"
        "\n"
        "nav {\n"
        "    margin: 20px 0;\n"
        "    padding: 10px 0;\n"
        "    border-bottom: 1px solid #eee;\n"
        "}\n"
        "\n"
        "nav a {\n"
        "    margin-right: 20px;\n"
        "    text-decoration: none;\n"
        "    color: #007acc;\n"
        "    font-weight: bold;\n"
        "}\n"
        "\n"
        "nav a:hover {\n"
        "    text-decoration: underline;\n"
        "}\n"
        "\n"
        ".content {\n"
        "    line-height: 1.6;\n"
        "}\n"
        "\n"
        "ul {\n"
        "    margin: 15px 0;\n"
        "}\n"
        "\n"
        "li {\n"
        "    margin: 8px 0;\n"
        "}\n"
        "\n"
        ".api-info {\n"
        "    background: #f8f9fa;\n"
        "    padding: 15px;\n"
        "    border-radius: 5px;\n"
        "    border-left: 4px solid #007acc;\n"
        "    margin: 20px 0;\n"
        "}\n"
        "\n"
        "code {\n"
        "    background: #f1f1f1;\n"
        "    padding: 2px 6px;\n"
        "    border-radius: 3px;\n"
        "    font-family: 'Courier New', monospace;\n"
        "}"
    );
    
    return response;
}

/**
 * @brief Serves JavaScript
 */
char *serve_js(void) {
    char *response = malloc(1024);
    if (!response) {
        perror("malloc failed in serve_js");
        return NULL;
    }
    
    strcpy(response,
        "// CExpress Static Server JavaScript\n"
        "console.log('CExpress static server loaded!');\n"
        "\n"
        "// Add some interactivity\n"
        "document.addEventListener('DOMContentLoaded', function() {\n"
        "    console.log('DOM loaded, adding interactivity...');\n"
        "    \n"
        "    // Add click handlers to navigation links\n"
        "    const navLinks = document.querySelectorAll('nav a');\n"
        "    navLinks.forEach(link => {\n"
        "        link.addEventListener('click', function(e) {\n"
        "            console.log('Navigating to:', this.href);\n"
        "        });\n"
        "    });\n"
        "    \n"
        "    // Add a simple animation to the container\n"
        "    const container = document.querySelector('.container');\n"
        "    if (container) {\n"
        "        container.style.opacity = '0';\n"
        "        container.style.transform = 'translateY(20px)';\n"
        "        \n"
        "        setTimeout(() => {\n"
        "            container.style.transition = 'all 0.5s ease';\n"
        "            container.style.opacity = '1';\n"
        "            container.style.transform = 'translateY(0)';\n"
        "        }, 100);\n"
        "    }\n"
        "});\n"
        "\n"
        "// Simple utility function\n"
        "function showMessage(message) {\n"
        "    alert('CExpress: ' + message);\n"
        "}"
    );
    
    return response;
}

/**
 * @brief Serves API information as JSON
 */
char *serve_api_info(void) {
    char *response = malloc(1024);
    if (!response) {
        perror("malloc failed in serve_api_info");
        return NULL;
    }
    
    time_t now = time(NULL);
    snprintf(response, 1024,
        "{\n"
        "  \"server\": \"CExpress\",\n"
        "  \"version\": \"1.0.0\",\n"
        "  \"timestamp\": %ld,\n"
        "  \"endpoints\": [\n"
        "    {\n"
        "      \"path\": \"/\",\n"
        "      \"method\": \"GET\",\n"
        "      \"description\": \"Home page\"\n"
        "    },\n"
        "    {\n"
        "      \"path\": \"/about\",\n"
        "      \"method\": \"GET\",\n"
        "      \"description\": \"About page\"\n"
        "    },\n"
        "    {\n"
        "      \"path\": \"/api/info\",\n"
        "      \"method\": \"GET\",\n"
        "      \"description\": \"API information\"\n"
        "    },\n"
        "    {\n"
        "      \"path\": \"/static/style.css\",\n"
        "      \"method\": \"GET\",\n"
        "      \"description\": \"CSS styles\"\n"
        "    },\n"
        "    {\n"
        "      \"path\": \"/static/script.js\",\n"
        "      \"method\": \"GET\",\n"
        "      \"description\": \"JavaScript code\"\n"
        "    }\n"
        "  ]\n"
        "}"
    );
    
    return response;
}

int main(void) {
    printf("Starting CExpress Static File Server...\n");
    
    // Initialize server on port 8080, max 15 clients, backlog 5, DEV mode
    Server *server = server_init(8080, 15, 5, DEV);
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }
    
    printf("Server initialized successfully\n");
    printf("Listening on http://localhost:8080\n");
    printf("Press Ctrl+C to stop the server\n\n");
    
    // Add routes for static content
    if (!server_add_route(server, GET, "/", serve_index)) {
        fprintf(stderr, "Failed to add GET / route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/about", serve_about)) {
        fprintf(stderr, "Failed to add GET /about route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/api/info", serve_api_info)) {
        fprintf(stderr, "Failed to add GET /api/info route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/static/style.css", serve_css)) {
        fprintf(stderr, "Failed to add GET /static/style.css route\n");
        server_free(server);
        return 1;
    }
    
    if (!server_add_route(server, GET, "/static/script.js", serve_js)) {
        fprintf(stderr, "Failed to add GET /static/script.js route\n");
        server_free(server);
        return 1;
    }
    
    printf("Static File Routes registered:\n");
    printf("  GET /                 - Home page (HTML)\n");
    printf("  GET /about           - About page (HTML)\n");
    printf("  GET /api/info        - API information (JSON)\n");
    printf("  GET /static/style.css - CSS styles\n");
    printf("  GET /static/script.js - JavaScript code\n\n");
    
    printf("Open your browser and visit: http://localhost:8080\n");
    printf("Or test with curl:\n");
    printf("  curl http://localhost:8080/\n");
    printf("  curl http://localhost:8080/about\n");
    printf("  curl http://localhost:8080/api/info\n\n");
    
    // Start the server (this blocks until Ctrl+C)
    int result = server_start(server);
    
    if (result == -1) {
        fprintf(stderr, "Server failed to start\n");
        return 1;
    }
    
    printf("Server stopped gracefully\n");
    return 0;
}
