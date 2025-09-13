# API Reference

## Types

### `Server`
Main server structure containing socket, address, client list, and router list.

### `client_t`
Represents a connected client with socket and address info.

### `Mode`
Server binding mode:
- `DEV` - localhost only (127.0.0.1)
- `PROD` - all interfaces (0.0.0.0)

### `method_t`
HTTP methods: `GET`, `POST`, `PUT`, `DELETE`, `FAIL`

### `HandlerFunc`
Function pointer type for route handlers:
```c
typedef char *(*HandlerFunc)(void);
```
**Note**: Returned `char *` is automatically freed by the framework.

## Functions

### `server_init(port, max_clients, backlog, mode)`
```c
Server *server_init(int port, int max_clients, int backlog, Mode mode);
```
Creates and initializes a new server instance.
- **Returns**: `Server *` on success, `NULL` on failure
- **Parameters**: port number, max concurrent clients, connection queue size, binding mode

### `server_start(server)`
```c
int server_start(Server *server);
```
Starts the server and begins accepting connections.
- **Returns**: `1` on successful shutdown, `-1` on error
- **Note**: This function blocks until interrupted
- **Signal Handling**: Automatically handles SIGINT (Ctrl+C) to free all resources and shutdown gracefully

### `server_free(server)`
```c
void server_free(Server *server);
```
Frees all server resources and shuts down the server.
- **Note**: Only use for error handling or if `server_start()` was not called

### `server_add_route(server, method, path, handler)`
```c
int server_add_route(Server *server, method_t method, path_t path, HandlerFunc handler);
```
Registers a new route with a handler function.
- **Returns**: `1` on success, `0` on failure
- **Parameters**: server instance, HTTP method, URL path, handler function

### `server_remove_route(server, method, path)`
```c
int server_remove_route(Server *server, method_t method, path_t path);
```
Removes a route from the server.
- **Returns**: `1` if removed, `0` if not found

## Usage

```c
#include "include/CExpress/server.h"

char *hello_handler(void) {
    char *response = malloc(50);
    strcpy(response, "Hello World!");
    return response;
}

int main(void) {
    Server *server = server_init(8080, 10, 5, DEV);
    server_add_route(server, GET, "/hello", hello_handler);
    server_start(server);
    return 0;
}
```
