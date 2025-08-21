# Custom REST API in C

> Note: This is an AI generated checklist that will be modified as time goes on

## Project Overview
This project is a **custom REST API server implemented in C** from scratch.  
Unlike using frameworks like Express.js or Flask, this project builds the **core HTTP server, routing, and request handling manually**, demonstrating deep backend and systems programming knowledge.

The goal is to have a **tangible, deployable API** that can serve as a foundation for future C-based backend projects or applications.

---

## Why This Project
- Demonstrates **low-level backend skills**: sockets, concurrency, memory management.
- Shows understanding of **HTTP protocol and API design**.
- Produces a **real, interactive project** for recruiters to test (via API or frontend).
- Can evolve into a **reusable C web framework** for other projects.

---

## What You Will Build
- A TCP server in C that:
  - Accepts HTTP requests (GET, POST, PUT, DELETE).
  - Parses headers and request body.
  - Sends proper HTTP responses (status codes, headers, JSON body).
- Supports **basic routing** and **CRUD operations** for an example resource (e.g., tasks, notes, or links).
- Optional advanced features:
  - Thread pool for handling multiple clients.
  - File or SQLite storage for persistence.
  - JWT authentication for protected endpoints.

---

## Required Knowledge / Resources
### C Fundamentals
- Pointers, dynamic memory (`malloc`/`free`), structs, modular code.
- Resource: [Learn C - GeeksforGeeks](https://www.geeksforgeeks.org/c-programming-language/)

### Networking & Sockets
- TCP sockets: `socket()`, `bind()`, `listen()`, `accept()`, `recv()`, `send()`.
- Resource: [Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/)

### HTTP Protocol
- Request/response format, methods, status codes, headers.
- Resource: [MDN HTTP Basics](https://developer.mozilla.org/en-US/docs/Web/HTTP)

### Concurrency
- POSIX threads (`pthread`), or `select()` for I/O multiplexing.
- Resource: [POSIX Threads Tutorial](https://www.gnu.org/software/libc/manual/html_node/Threads.html)

### JSON Handling
- Manual string building or lightweight library like **cJSON**.
- Resource: [cJSON GitHub](https://github.com/DaveGamble/cJSON)

### Deployment
- Linux VPS (DigitalOcean, Render, Railway)
- Compile with `gcc`, optionally run as a service with `systemd`.
- Optional: Simple React frontend or Postman collection for testing.

---

## Checklist of Goals

### Phase 1: Setup & Basics
- [ ] Setup project structure (`server.c`, `router.c/h`, `handlers.c/h`, `Makefile`)
- [ ] Compile and run a basic TCP server
- [ ] Test server listens on a port

### Phase 2: HTTP Request Handling
- [ ] Accept connections and read HTTP requests
- [ ] Parse HTTP request line (method, path, version)
- [ ] Parse headers
- [ ] Handle request body for POST/PUT

### Phase 3: Routing & Responses
- [ ] Implement basic routing (`/tasks`, `/notes`, etc.)
- [ ] Return correct HTTP status codes (200, 404, 500)
- [ ] Return JSON responses

### Phase 4: CRUD Operations
- [ ] Implement in-memory storage (linked list or array)
- [ ] Support GET, POST, PUT, DELETE endpoints
- [ ] Test CRUD operations with curl/Postman

### Phase 5: Concurrency & Robustness
- [ ] Support multiple clients (threads or select/poll)
- [ ] Add proper error handling and logging
- [ ] Optional: add thread pool

### Phase 6: Optional Enhancements
- [ ] Persistent storage (file or SQLite)
- [ ] JWT authentication for protected endpoints
- [ ] Rate limiting
- [ ] Simple frontend demo (React or static HTML)
- [ ] Deploy server on VPS and make API public

---

## Outcome
By the end of this project, you will have:
- A **working REST API in C** that can handle multiple clients and CRUD operations.
- A **modular codebase** suitable for extension or reuse in other C projects.
- A **deployed, interactive demo** that recruiters can try.

---

## References
- [Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/)
- [MDN Web Docs: HTTP Basics](https://developer.mozilla.org/en-US/docs/Web/HTTP)
- [cJSON GitHub](https://github.com/DaveGamble/cJSON)
- [POSIX Threads Manual](https://www.gnu.org/software/libc/manual/html_node/Threads.html)
- [Tiny HTTP Server Examples in C](https://github.com/jeremycw/httpserver)

