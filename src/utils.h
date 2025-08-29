#pragma once

typedef enum {GET, POST, PUT, DELETE} method_t;

typedef char * path_t;

typedef void (*HandlerFunc)(int client_sock, const char *request);

