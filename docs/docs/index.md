# CExpress

A lightweight HTTP server framework for C that brings the simplicity of Express.js to the power and performance of C.

## What is CExpress?

CExpress is a minimal, high-performance HTTP server framework written in C. It provides Express.js-like functionality for building web applications and APIs with the speed and efficiency of C. 

Perfect for microservices, APIs, static file serving, and rapid prototyping without the overhead of larger frameworks.


**Perfect for:**
- Microservices and API development
- Static file hosting
- Rapid prototyping
- Embedded web interfaces

## Key Features

- **Fast** - Built on efficient C socket programming
- **Simple** - Express.js-inspired API design
- **Lightweight** - Minimal dependencies and memory footprint
- **Cross-platform** - Works on Linux and macOS

## Quick Example

```c
#include <CExpress/server.h>
#include <stdio.h>
#include <stdlib.h>

char *hello_handler(void) {
    char *response = malloc(50);
    strcpy(response, "Hello from CExpress!");
    return response;
}

int main(void) {
    Server *server = server_init(8080, 10, 5, DEV);
    server_add_route(server, GET, "/hello", hello_handler);
    server_start(server);
    return 0;
}
```

## What's Next?

- **[Installation](installation)** - Install CExpress
- **[API Reference](api-reference)** - Complete function documentation
- **[Usage Examples](usage-examples)** - See CExpress in action
