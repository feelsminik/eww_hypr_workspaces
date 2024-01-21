#include <cjson/cJSON.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/_daemon.h"
#include "../include/_debug.h"
#include "../include/_hypr.h"
#include "../include/_hypr_structs.h"
#include "../include/_macros.h"
#include "../include/_socket.h"

int main(void) {
  char hypr_socket_buffer[MAX_BUFFER_SIZE];
  char socket_path[128] = "";

  hypr_socket_path(socket_path, sizeof(socket_path));
  int sock_fd = create_socket(socket_path);

  while (1) {
    // Recieve socket bytes
    read_from_socket(sock_fd, hypr_socket_buffer, MAX_BUFFER_SIZE);

    // Maybe update eww show_workspace variable - see go impl

    if (is_changing_workspace(hypr_socket_buffer) == true) {
      workspaceTile workspace_tiles[MAX_NUM_OF_WORKSPACES + 1] = {{}};
      get_hypr_workspaces(workspace_tiles);
      printWorkspaceTiles(workspace_tiles);
      printf("======\n");
    }

    // Empty buffer for new retrieval
    memset(hypr_socket_buffer, 0, sizeof(hypr_socket_buffer));
  }

  close_socket(sock_fd);
  return 0;
}
