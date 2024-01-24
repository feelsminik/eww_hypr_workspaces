#include <cjson/cJSON.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/_debug.h"
#include "../include/_hypr_structs.h"
#include "../include/_macros.h"

// Create path to hypr socket
char* hypr_socket_path(char* socket_path, size_t socket_path_size) {
  char* hypr_signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");
  snprintf(socket_path, socket_path_size, "/tmp/hypr/%s/.socket2.sock",
           hypr_signature);
  return socket_path;
}

// Takes in eventlines produced by Hypr Daemon and returns true if an event
// associated to changing workspace is fired
bool is_changing_workspace(char* eventline) {
  return strncmp("movetoworkspace", eventline, strlen("movetoworkspace")) ==
             0 ||
         strncmp("workspace", eventline, strlen("workspace")) == 0 ||
         strncmp("focusedmon", eventline, strlen("focusedmon")) == 0;
}

int get_number_by_key(cJSON* json, char* key) {
  return cJSON_GetNumberValue(cJSON_GetObjectItem(json, key));
}

int find_special_workspace(cJSON* unfiltered_workspaces) {
  int num_of_workspaces = cJSON_GetArraySize(unfiltered_workspaces);
  for (size_t i = 0; i < num_of_workspaces; i++) {
    struct cJSON* workspace = cJSON_GetArrayItem(unfiltered_workspaces, i);
    int workspace_id = get_number_by_key(workspace, "id");
    if (workspace_id == -99) {
      return i;
    }
  }
  return -1;
}

void filter_out_special_workspace(cJSON* unfiltered_workspaces) {
  int special_workspace_index = find_special_workspace(unfiltered_workspaces);
  if (special_workspace_index != -1) {
    cJSON_DeleteItemFromArray(unfiltered_workspaces, special_workspace_index);
  }
}

cJSON* get_hyprctl_output(char* command) {
  FILE* output_file = popen(command, "r");
  char output_buffer[MAX_BUFFER_SIZE] = {'0'};

  if (output_file == NULL) {
    perror("Reading hyprctl output failed");
    exit(EXIT_FAILURE);
  } else {
    size_t bytes_read =
        fread(output_buffer, sizeof(char), MAX_BUFFER_SIZE, output_file);
    output_buffer[bytes_read] = '\0';
  }
  pclose(output_file);
  // might cause memory leak, maybe delete
  struct cJSON* output_json = cJSON_Parse(output_buffer);
  filter_out_special_workspace(output_json);

  return output_json;
}

void get_hypr_workspaces(workspaceTile* workspace_tiles,
                         size_t* count_workspaces_per_monitor) {
  struct cJSON* workspaces = get_hyprctl_output("/bin/hyprctl workspaces -j");
  struct cJSON* active_workspace =
      get_hyprctl_output("/bin/hyprctl activeworkspace -j");
  int active_workspace_id = get_number_by_key(active_workspace, "id");
  int num_of_workspaces = cJSON_GetArraySize(workspaces);

  if (num_of_workspaces >= MAX_NUM_OF_WORKSPACES) {
    perror("found more workspaces, than supported.");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < num_of_workspaces; i++) {
    struct cJSON* workspace = cJSON_GetArrayItem(workspaces, i);
    int workspace_id = get_number_by_key(workspace, "id");
    int monitor_id = get_number_by_key(workspace, "monitorID");

    if (monitor_id + 1 > MAX_NUM_OF_MONITORS) {
      perror("found more monitors, than supported.");
      exit(EXIT_FAILURE);
    }

    count_workspaces_per_monitor[monitor_id]++;
    workspace_tiles[i].index = workspace_id;
    workspace_tiles[i].monitor_id = monitor_id;
    sprintf(workspace_tiles[i].css_class, "mon-%d", monitor_id);
    if (workspace_id == active_workspace_id) {
      strcpy(workspace_tiles[i].icon, "󰮯");
    } else {
      strcpy(workspace_tiles[i].icon, "󰊠");
    }
  }
  cJSON_Delete(workspaces);
}
