#include <cjson/cJSON.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/_daemon.h"
#include "../include/_debug.h"
#include "../include/_hypr.h"
#include "../include/_socket.h"

#define MAX_BUFFER_SIZE 2048

int main(void) {
  char hypr_socket_buffer[MAX_BUFFER_SIZE];
  char socket_path[128] = "";

  hypr_socket_path(socket_path, sizeof(socket_path));
  int sock_fd = create_socket(socket_path);

  while (1) {
    // Recieve socket bytes
    read_from_socket(sock_fd, hypr_socket_buffer, MAX_BUFFER_SIZE);

    if (is_changing_workspace(hypr_socket_buffer) == true) {
      struct cJSON *monitors = get_hypr_monitors();
      int num_of_monitors = cJSON_GetArraySize(monitors);
      int monitor_ids[3] = {0};
      for (int i = 0; i < num_of_monitors; i++) {
        struct cJSON *monitor = cJSON_GetArrayItem(monitors, i);
        struct cJSON *id = cJSON_GetObjectItem(monitor, "id");
        monitor_ids[i] = cJSON_GetNumberValue(id);
      }
      printIntArray(monitor_ids);
      cJSON_Delete(monitors);

      // Maybe update eww show_workspace variable - see go impl
      printf("%s", hypr_socket_buffer);
      // Continue with popen()
    }

    // Empty buffer for new retrieval
    memset(hypr_socket_buffer, 0, sizeof(hypr_socket_buffer));
  }

  close_socket(sock_fd);
  return 0;
}
