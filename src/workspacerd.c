#include <cjson/cJSON.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/_hypr.h"
#include "../include/_macros.h"
#include "../include/_socket.h"

int main(void) {
  setbuf(stdout, NULL);
  char hypr_socket_buffer[MAX_BUFFER_SIZE];
  char socket_path[128] = "";

  hypr_socket_path(socket_path, sizeof(socket_path));
  int sock_fd = create_socket(socket_path);

  while (1) {
    // Recieve socket bytes
    read_from_socket(sock_fd, hypr_socket_buffer, MAX_BUFFER_SIZE);
    if (is_changing_workspace(hypr_socket_buffer) == true) {
      cJSON *workspace_infos = get_hypr_workspaces();
      system("eww update show_workspaces=false");
      printf("%s\n", cJSON_PrintUnformatted(workspace_infos));
      system("eww update show_workspaces=true");
      cJSON_Delete(workspace_infos);
    }

    // Empty buffer for new retrieval
    memset(hypr_socket_buffer, 0, sizeof(hypr_socket_buffer));
  }

  close_socket(sock_fd);
  return 0;
}
