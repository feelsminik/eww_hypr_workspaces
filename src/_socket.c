#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

int create_socket(char* socket_path) {
  // Create empty socket
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("socket creation failed.");
    exit(EXIT_FAILURE);
  }

  // Create socket address
  struct sockaddr_un server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  // Takes path to socket and returns sock_fd
  strcpy(server_addr.sun_path, socket_path);

  // Connect to socket with empty socket and socket address
  int sock_connection =
      connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (sock_connection == -1) {
    perror("socket connection failed.");
    exit(EXIT_FAILURE);
  }

  return sock_fd;
}

void close_socket(int sock_fd) { close(sock_fd); }

void read_from_socket(int sock_fd, char* buffer, size_t buffer_size) {
  ssize_t bytesRead = recv(sock_fd, buffer, buffer_size, 0);

  if (bytesRead == -1) {
    perror("socket connection failed.");
    close_socket(sock_fd);
    exit(EXIT_FAILURE);
  } else if (bytesRead == 0) {
    perror("connection closed.");
    exit(EXIT_SUCCESS);
  } else {
    buffer[bytesRead] = '\0';
  }
}
