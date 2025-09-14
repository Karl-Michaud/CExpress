# 🚀 CExpress

> *Because C deserves frameworks as simple as Express.js* 

**A lightweight HTTP server framework for C** that brings the simplicity of Express.js to the power and performance of C.

CExpress is a minimal, high-performance HTTP server framework written in C that provides Express.js-like functionality for building web applications and APIs. Perfect for microservices, APIs, static file serving, and rapid prototyping without the overhead of larger frameworks.

📖 Full documentation available at: [CExpress Docs](https://karl-michaud.github.io/CExpress)  

## 📚 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Installation](#installation)
- [Quick Start](#-quick-start)
- [Examples](#-examples)
- [API Reference](#-api-reference)
- [Architecture](#architecture)
- [Performance](#-performance)
- [Contributing](#-contributing)
- [License](#-license)

## 📖 Overview

CExpress brings the familiar Express.js patterns to C development:

- **Rapid Prototyping**: Build and test instantly  
- **Simple routing**: GET, POST, PUT, DELETE  
- **Handler-based architecture**: Modular, clean code  
- **Static file serving**: Out of the box  
- **JSON API support**: Built-in response formatting  
- **Cross-platform**: Linux and macOS

---

## ✨ Features

### 🎯 Core Features
- **Lightweight**: Minimal memory footprint and dependencies
- **Fast**: Built on efficient C socket programming
- **Simple**: Express.js-inspired API design
- **Flexible**: Support for various use cases and patterns

### 🔧 Technical Features
- **HTTP/1.1 compliant** request/response handling
- **Dynamic routing** with method and path matching
- **Memory management** with automatic cleanup
- **Error handling** with proper HTTP status codes
- **Signal handling** for graceful shutdown
- **Concurrent client** support with select() multiplexing

### 🌐 Supported Use Cases
- REST APIs and microservices
- Static file hosting
- Web application backends
- Rapid development servers
- Prototyping and testing
- Embedded web interfaces

---

## Installation

CExpress can be installed as a shared C library in two ways:

### 1. Install from GitHub (macOS & Linux)

#### Prerequisites

- GCC or Clang compiler
- Make utility

#### Install

```bash
git clone https://github.com/Karl-Michaud/CExpress.git
cd CExpress
make install
```

This will:
- Copy the shared library (`libCExpress.dylib` on macOS, or `libCExpress.so` on Linux) to `/usr/local/lib/`
- Copy the header files to `/usr/local/include/CExpress/`

#### Uninstall

To remove CExpress, run:

```bash
make uninstall
```

#### Notes

- To install to a different location, set the `PREFIX` variable:
  ```bash
  make install PREFIX=/custom/path
  ```
- For Linux, the shared library extension is `.so`; for macOS, it is `.dylib`.


### 2. Install via Homebrew (macOS only... coming soon)

Homebrew support is planned for **macOS**.  
When available, you will be able to install with:

```bash
brew install karl-michaud/cexpress/cexpress
```

#### Uninstall

If you installed CExpress via Homebrew (macOS only), you can uninstall it with:

```bash
brew uninstall cexpress
```

This will remove the CExpress library and headers installed by Homebrew.

---

## 🚀 Quick Start

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
    // Initialize server
    Server *server = server_init(8080, 10, 5, DEV);
    if (!server) {
        fprintf(stderr, "Failed to initialize server\n");
        return 1;
    }
    
    // Add route
    server_add_route(server, GET, "/hello", hello_handler);
    
    // Start server
    server_start(server);
    
    // Cleanup (automatic on shutdown)
    return 0;
}
```

### Compile and Run

```bash
gcc -o server main.c -lCExpress
./server
```

Test with:
```bash
curl http://localhost:8080/hello
```

---

## 📝 Examples

Found in [examples/](https://github.com/Karl-Michaud/CExpress/tree/main/examples)

1) **Hello World** → minimal GET route
2) **JSON API** → Multi-method API with JSON responses, error handling, and data management.
3) **Static Files** → serve HTML, CSS, JS
4) **REST API** → Full CRUD operations with proper REST patterns and status codes.

---

## 📖 API Reference

### Server Management

```c
// Initialize server
Server *server_init(int port, int max_clients, int backlog, Mode mode);

// Start server (blocks until shutdown)
int server_start(Server *server);

// Clean up resources
void server_free(Server *server);
```

### Routing

```c
// Add route
int server_add_route(Server *server, method_t method, path_t path, HandlerFunc handler);

// Remove route
int server_remove_route(Server *server, method_t method, path_t path);
```

### Handler Functions

```c
// Handler function signature
typedef char *(*HandlerFunc)(void);

// Example handler
char *my_handler(void) {
    char *response = malloc(100);
    strcpy(response, "Hello World!");
    return response;
}
```

### HTTP Methods

```c
typedef enum {GET, POST, PUT, DELETE, FAIL} method_t;
```

### Server Modes

```c
typedef enum {DEV, PROD} Mode;
// DEV  - localhost only (127.0.0.1)
// PROD - all interfaces (0.0.0.0)
```

---

## Architecture

CExpress follows a modular architecture:

```
┌─────────────────┐
│   Public API    │  ← server.h
├─────────────────┤
│   Server Core   │  ← server.c
├─────────────────┤
│   Routing       │  ← routers.c/h
├─────────────────┤
│   Handlers      │  ← handlers.c/h
├─────────────────┤
│   Utilities     │  ← utils.c/h
└─────────────────┘
```

### Request Flow

1. **Client Request** → Server Socket
2. **Accept Connection** → Client List
3. **Read Data** → Buffer
4. **Parse Header** → Extract Method/Path
5. **Find Route** → Router List
6. **Execute Handler** → Generate Response
7. **Send Response** → Client
8. **Cleanup** → Memory Management

---

## ⚡ Performance

CExpress is designed for performance:

- **Low Latency**: Direct socket programming
- **Memory Efficient**: Minimal allocations and automatic cleanup
- **Concurrent**: Multiple client support
- **Lightweight**: Small binary size and memory footprint
- **Rapid Prototyping**: Instantly build and test web applications without the overhead of large frameworks

---

## 🤝 Contributing

I welcome contributions! Here's how to get started:

### Development Setup

1. Fork the repository & create a feature branch
2. Make your changes & add tests if applicable
3. Submit a PR 🚀

### Areas for Contribution

- Additional HTTP methods
- Middleware support
- WebSocket support
- Enhanced error handling
- Performance optimizations
- Additional examples

---

## 📄 License

MIT License

Copyright (c) 2025 Karl-Alexandre Michaud

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---

**Made by Karl-Alexandre Michaud**

*Because C deserves frameworks as simple as Express.js*
