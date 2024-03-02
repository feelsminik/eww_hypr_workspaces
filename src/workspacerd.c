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
      size_t count_workspaces_per_monitor[MAX_NUM_OF_WORKSPACES] = {0};
      get_hypr_workspaces(workspace_tiles, count_workspaces_per_monitor);

      size_t left_monitor_workspaces = count_workspaces_per_monitor[0];
      workspaceTile left_monitor_tiles[left_monitor_workspaces];
      memset(left_monitor_tiles, 0,
             left_monitor_workspaces * sizeof(workspaceTile));

      size_t right_monitor_workspaces = count_workspaces_per_monitor[1];
      workspaceTile right_monitor_tiles[right_monitor_workspaces];
      memset(right_monitor_tiles, 0,
             right_monitor_workspaces * sizeof(workspaceTile));

      int left_added = 0;
      int right_added = 0;
      size_t amount_of_all_workspaces =
          left_monitor_workspaces + right_monitor_workspaces;
      for (size_t i = 0; i < amount_of_all_workspaces; i++) {
        if (workspace_tiles[i].monitor_id == 0) {
          memcpy(&left_monitor_tiles[left_added++], &workspace_tiles[i],
                 sizeof(workspaceTile));
        } else if (workspace_tiles[i].monitor_id == 1) {
          memcpy(&right_monitor_tiles[right_added++], &workspace_tiles[i],
                 sizeof(workspaceTile));
        } else if (workspace_tiles[i].monitor_id > 1) {
          perror("found monitor_id greater than supported.");
          exit(EXIT_FAILURE);
          // close socket here
        }
      }

      memcpy(&workspace_tiles[0], &left_monitor_tiles[0],
             left_added * sizeof(workspaceTile));

      workspace_tiles[left_added].index = -1;
      sprintf(workspace_tiles[left_added].css_class, "splitter");
      strcpy(workspace_tiles[left_added].icon, "󱖢");

      memcpy(&workspace_tiles[left_added + 1], &right_monitor_tiles[0],
             right_added * sizeof(workspaceTile));

      cJSON *workspace_infos = cJSON_CreateArray();
      for (size_t i = 0; i <= MAX_NUM_OF_WORKSPACES; i++) {
        if (workspace_tiles[i].index != 0) {
          cJSON *workspace_info = cJSON_CreateObject();
          cJSON *index = cJSON_CreateNumber(workspace_tiles[i].index);
          cJSON *monitor_id = cJSON_CreateNumber(workspace_tiles[i].monitor_id);
          cJSON *icon = cJSON_CreateString(workspace_tiles[i].icon);
          cJSON *css = cJSON_CreateString(workspace_tiles[i].css_class);

          cJSON_AddItemToObject(workspace_info, "index", index);
          cJSON_AddItemToObject(workspace_info, "monitor_id", monitor_id);
          cJSON_AddItemToObject(workspace_info, "icon", icon);
          cJSON_AddItemToObject(workspace_info, "css_class", css);

          cJSON_AddItemToArray(workspace_infos, workspace_info);
        }
      }

      FILE *test = popen("/usr/bin/eww update show_workspaces=false", "r");
      system("/usr/bin/eww update show_workspaces=false");
      printf("%s\n", cJSON_PrintUnformatted(workspace_infos));
      FILE *test1 = popen("/usr/bin/eww update show_workspaces=true", "r");
      fflush(stdout);
      cJSON_Delete(workspace_infos);

      pclose(test1);
      pclose(test);
    }

    // Empty buffer for new retrieval
    memset(hypr_socket_buffer, 0, sizeof(hypr_socket_buffer));
  }

  close_socket(sock_fd);
  return 0;
}
