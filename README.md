# Student Chat - Multithreaded TCP Client-Server in C

## 1. Introduction

This project implements a client-server chat system using TCP sockets and POSIX threads (`pthread`) in C. The system enables bidirectional communication by exchanging a custom `Student` struct, which contains:
- `id`: An integer representing the student's ID.
- `name`: A character array (up to 50 characters) for the student's name.
- `score`: A floating-point number for the student's score.

Key features:
- The server (`server_chat.c`) listens for client connections on port 8080.
- The client (`client_chat.c`) connects to the server and initiates communication.
- Both client and server use separate threads for sending and receiving `Student` structs, allowing asynchronous, concurrent data exchange.
- The system supports real-time communication on the same machine (localhost) or across a network with proper IP configuration.

## 2. Compilation

To compile the programs, ensure you have a C compiler (e.g., `gcc`) and `pthread` library support on a POSIX-compliant system (e.g., Linux).

1. Open a terminal and navigate to the directory containing the source files (`server_chat.c` and `client_chat.c`).
2. Compile the server:
   ```bash
   gcc server_chat.c -o server -lpthread
   ```
3. Compile the client:
   ```bash
   gcc client_chat.c -o client -lpthread
   ```

The `-lpthread` flag links the POSIX threads library, required for multithreading.

## 3. Running the Program

### Start the Server
1. Run the server executable:
   ```bash
   ./server
   ```
2. The server will bind to port 8080 and display:
   ```
   Server waiting for connection...
   ```

### Start the Client
1. In a separate terminal, run the client executable:
   ```bash
   ./client
   ```
2. The client connects to the server at `127.0.0.1:8080` (localhost) and displays:
   ```
   Connected to server.
   ```
3. The server confirms the connection:
   ```
   Client connected.
   ```

**Note**: For network communication (different machines), modify the server IP address in `client_chat.c` (default: `127.0.0.1`) to the server's actual IP.

## 4. Usage

Both server and client operate similarly:
- The main thread prompts the user to input `Student` data:
  ```
  > Enter ID: (e.g., 101)
    Enter Name: (e.g., John Doe)
    Enter Score: (e.g., 8.5)
  ```
- After input, the data is packaged into a `Student` struct and sent to the other side via TCP.
- A dedicated receiver thread asynchronously listens for incoming `Student` structs and prints them.

**Example Output**:
- Server receiving data from client:
  ```
  [From Client] ID=102, Name=Jane Smith, Score=7.25
  ```
- Client receiving data from server:
  ```
  [From Server] ID=101, Name=John Doe, Score=8.50
  ```

The receiver thread ensures that incoming messages are displayed immediately, even while the user is inputting new data.

## 5. Terminating the Program

- **Manual Termination**: Press `Ctrl+C` in the terminal to stop either the server or client.
- **Connection Loss**: If one side terminates (e.g., server closes), the other side detects the disconnection:
  - Client displays: `Server disconnected.`
  - Server displays: `Client disconnected.`
  - The receiver thread exits, and the program terminates.

## 6. Technical Details

- **Socket Programming**: The system uses TCP sockets (`AF_INET`, `SOCK_STREAM`) for reliable, connection-oriented communication.
- **Multithreading**: Each side spawns a `pthread` to handle incoming messages, allowing simultaneous sending and receiving without blocking the main thread.
- **Data Structure**:
  ```c
  typedef struct {
      int id;
      char name[50];
      float score;
  } Student;
  ```
- **Port**: The server listens on port 8080, configurable in the source code if needed.
- **Error Handling**: Basic checks are implemented for socket creation, binding, and connection failures. However, input validation is minimal.

## 7. Notes

- **Input Constraints**: Avoid invalid inputs (e.g., non-numeric IDs, names longer than 49 characters) to prevent undefined behavior.
- **Localhost vs. Network**: The default configuration uses `127.0.0.1`. For network use, update the IP in `client_chat.c` and ensure the server is accessible (firewall settings may apply).
- **Single Client**: The server supports one client at a time. Multi-client support requires additional socket handling (e.g., using `select` or `fork`).
- **Thread Safety**: The current implementation is thread-safe for a single connection, as sending and receiving use a single socket.

## 8. Limitations

- **Error Handling**: Lacks robust input validation (e.g., buffer overflows in `scanf`) and advanced error recovery.
- **Single Client**: Only one client can connect at a time.
- **Graceful Shutdown**: No mechanism to cleanly terminate threads or sockets without abrupt closure.
- **Scalability**: Not optimized for high-throughput or multiple simultaneous connections.

## 9. Future Improvements

- Implement multi-client support using `select`, `poll`, or separate threads per client.
- Add a message protocol to distinguish between data types or control messages (e.g., connect, disconnect).
- Enhance input validation to prevent crashes from malformed inputs.
- Introduce timeout mechanisms for idle connections.
- Add a graceful shutdown procedure to clean up threads and sockets.