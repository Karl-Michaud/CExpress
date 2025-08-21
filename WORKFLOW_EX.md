# CExpress Workflow

This document explains how a request flows through **CExpress**, from the moment a user sends it to the moment they receive a response. Understanding this workflow will help contributors and recruiters see how the library works internally and how users interact with it.

---

## **1. User Sends a Request**

A client (browser, Postman, or another app) sends an HTTP request to the server:

```
GET http://localhost:8080/hello
```

At this point, the server has not yet processed the request; it’s just arriving at the TCP layer.

---

## **2. server.c: Accepting the Connection**

- The server (implemented in `cexpress.c`) listens for incoming connections using:
  - `socket()`, `bind()`, `listen()`, `accept()`
- Once a connection is accepted:
  - `recv()` reads the raw HTTP request from the client
  - The server creates a `request_t` struct containing:
    - HTTP method (GET, POST, etc.)
    - Path (`/hello`)
    - Body (if applicable)

**Files involved:** `cexpress.c`, `cexpress.h`  

---

## **3. utils.c: Parsing the Request**

- The raw HTTP request is parsed using helper functions from `utils.c`:
  - Split request line into method, path, and version
  - Parse headers
  - Extract body if present
- Result: a clean `request_t` object that can be routed

**Files involved:** `utils.c`, `utils.h`  

---

## **4. router.c: Matching Routes**

- The router receives the `request_t` object
- It searches the **route table** for a match:
  - Path (e.g., `/hello`)
  - HTTP method (GET, POST, etc.)
- If a match is found:
  - The router retrieves a pointer to the registered handler function
- If no match is found:
  - The router calls a default 404 handler

**Files involved:** `router.c`, `router.h`  

---

## **5. handlers.c: Processing the Request**

- The matched handler is called:

```
matched_handler(&request, &response);
```

- Handler responsibilities:
  - Perform the business logic (e.g., read tasks, create a resource)
  - Build the response in a `response_t` struct:
    - `status_code` (200, 404, 500, etc.)
    - `body` (JSON, text, etc.)
  - Optionally use `utils.c` helpers to format JSON or log actions

**Files involved:** `handlers.c`, `handlers.h`

---

## **6. cexpress.c: Sending the Response**

- The server takes the filled `response_t` struct
- Converts it into a proper HTTP response string:

```
HTTP/1.1 200 OK
Content-Type: application/json
Content-Length: 33

{"message": "Hello from CExpress!"}
```


- Sends it back to the client using `send()`

**Files involved:** `server.c`

---

## **7. Summary Flow Diagram**

User Request
     │
     ▼
cexpress.c (accept connection, recv)
     │
     ▼
utils.c (parse HTTP request → request_t)
     │
     ▼
router.c (match path + method → handler)
     │
     ▼
handlers.c (process request → fill response_t)
     │
     ▼
utils.c (optional helpers: JSON, logging)
     │
     ▼
server.c (serialize & send HTTP response)
     │
     ▼
Client receives response

