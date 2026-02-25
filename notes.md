```<sys/socket.h> = socket(), bind(), listen(), accept(), send(), recv()
<netinet/in.h> = sockaddr_in (IPv4 structures)
<unistd.h> = close(), read()```

```int server_fd = socket(AF_INET, SOCK_STREAM, 0);```

Makes a TCP socket for IPv4

AF_INET = IPv4
SOCK_STREAM = TCP (streams, rather than UDP which sends packets into the void like a shotgun)
0 = default protocol

Returns:
File descriptor (int)
Or -1 on error

```sockaddr_in address{};```
This struct defines IP, Port, and Address Family

```address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;```
sin_family must match socket domain.

INADDR_ANY means bind to:
0.0.0.0
(Listen on all network interfaces)
If you set:
inet_addr("127.0.0.1")
It would only listen locally.

```address.sin_port = htons(8080);```
Very important.
Ports must be in network byte order (big-endian).

htons() =
Host To Network Short.
It converts Little-endian to Big-endian.
Without this, networking breaks.

```bind(server_fd, (sockaddr*)&address, sizeof(address));```
This tells the OS, "Attach this socket to port 8080 on the given IP"

Important syntax:
`(sockaddr*)&address`

C-style cast used here.

Cast because bind() expects a generic sockaddr*.
But we have a sockaddr_in*.

```listen(server_fd, 10);```
Backlog = 10 means Kernel can queue up to 10 pending connections before refusing new ones.
Socket now moves to PASSIVE mode.

```int client_fd = accept(server_fd, nullptr, nullptr);```
This:
-Blocks until a client connects
-Performs TCP handshake
-Returns a new socket FD

Important concept:
We now have:
server_fd: listening socket
client_fd: connection-specific socket
This is how servers handle multiple clients.

```char buffer[4096] = {0};
read(client_fd, buffer, sizeof(buffer));```
This reads raw bytes from TCP.

```const char* response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "\r\n"
    "Hello, world!";```

Pretty self-explanatory, manually write a response.
\r\n because HTTP requires CRLF line endings.

Empty line:
\r\n
Marks end of headers (then it's the body).
Content-Length must match body length; "Hello, world!" is 13 bytes.

```send(client_fd, response, strlen(response), 0);```
This writes bytes to the TCP socket.
Last parameter 0 means no flags.

After all this, we simply close the connections!