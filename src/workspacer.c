#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 256

bool isChangingWorkspace(char*);

int main(void) {
  /* int nochdir = 0; */
  /* int noclose = 0; */

  /* if (daemon(nochdir, noclose)) { */
  /*   perror("daemon failed."); */
  /*   exit(EXIT_FAILURE); */
  /* } */

  // Create empty socket
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("socket creation failed.");
    exit(EXIT_FAILURE);
  }

  // Create path to hypr socket
  char* hypr_signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");
  char server_path[128] = "";
  snprintf(server_path, sizeof(server_path), "/tmp/hypr/%s/.socket2.sock",
           hypr_signature);

  // Create socket address
  struct sockaddr_un server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  strcpy(server_addr.sun_path, server_path);

  // Connect to socket with empty sockte and socket address
  int sock_connection =
      connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (sock_connection == -1) {
    perror("socket connection failed.");
    exit(EXIT_FAILURE);
  }

  char buffer[MAX_BUFFER_SIZE];
  while (1) {
    // Recieve socket bytes
    ssize_t bytesRead = recv(sock_fd, buffer, sizeof(buffer), 0);

    if (bytesRead == -1) {
      perror("socket connection failed.");
      close(sock_fd);
      exit(EXIT_FAILURE);
    } else if (bytesRead == 0) {
      perror("connection closed.");
      exit(EXIT_SUCCESS);
    } else {
      buffer[bytesRead] = '\0';

      // Check for predefined event names
      if (isChangingWorkspace(buffer) == true) {
        printf("%s", buffer);
      }

      // Empty buffer for new retrieval
      memset(buffer, 0, sizeof(buffer));
    }
  }

  close(sock_fd);

  return 0;
}

bool isChangingWorkspace(char* eventline) {
  return strncmp("movetoworkspace", eventline, strlen("movetoworkspace")) ==
             0 ||
         strncmp("workspace", eventline, strlen("workspace")) == 0 ||
         strncmp("focusedmon", eventline, strlen("focusedmon")) == 0;
}
