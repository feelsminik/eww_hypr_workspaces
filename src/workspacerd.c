#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/_daemon.h"
#include "../include/_hypr.h"
#include "../include/_socket.h"

#define MAX_BUFFER_SIZE 256

int main(void) {
  char buffer[MAX_BUFFER_SIZE];
  char socket_path[128] = "";

  hypr_socket_path(socket_path, sizeof(socket_path));
  int sock_fd = create_socket(socket_path);

  while (1) {
    // Recieve socket bytes
    read_from_socket(sock_fd, buffer, MAX_BUFFER_SIZE);

    if (is_changing_workspace(buffer) == true) {
      printf("%s", buffer);
      // Continue with popen()
    }

    // Empty buffer for new retrieval
    memset(buffer, 0, sizeof(buffer));
  }

  close_socket(sock_fd);
  return 0;
}
