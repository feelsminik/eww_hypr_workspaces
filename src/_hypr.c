#include <cjson/cJSON.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/_macros.h"

// Create path to hypr socket
char* hypr_socket_path(char* socket_path, size_t socket_path_size) {
  char* hypr_signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");
  char* xdg_dir_path = getenv("XDG_RUNTIME_DIR");

  char xdg_hypr_dir_path[128] = "";
  snprintf(xdg_hypr_dir_path, 128, "%s/hypr", xdg_dir_path);
  DIR* xdg_hypr_dir = opendir(xdg_hypr_dir_path);
  if (xdg_hypr_dir) {
    closedir(xdg_hypr_dir);
    snprintf(socket_path, socket_path_size, "%s/%s/.socket2.sock",
             xdg_hypr_dir_path, hypr_signature);
  } else {
    snprintf(socket_path, socket_path_size, "/tmp/hypr/%s/.socket2.sock",
             hypr_signature);
  }

  return socket_path;
}

// Takes in eventlines produced by Hypr Daemon and returns true if an event
// associated to changing workspace is fired
bool is_changing_workspace(char* eventline) {
  return strstr(eventline, "movetoworkspace>>") != NULL ||
         strstr(eventline, "workspace>>") != NULL ||
         strstr(eventline, "focusedmon>>") != NULL;
}

static cJSON* get_hyprctl_output(char* command) {
  FILE* output_file = popen(command, "r");
  char output_buffer[MAX_BUFFER_SIZE] = {};

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
  return output_json;
}

cJSON* get_hypr_workspaces() {
  // [ [{id, icon}, {..}] <- monitor-0, [{id, icon}, {..}] <- monitor-1 ]
  struct cJSON* workspaces_per_monitor = get_hyprctl_output(
      "hyprctl workspaces -j |"
      "jq 'group_by(.monitorID) |"
      "map(map({ id, icon: \"\" }))|"   // [[],[]] => map(map())
      "map(map(select(.id != -99))) |"  // filter special workspace
      "map(sort_by(.id))'");

  int selected_workspace = get_hyprctl_output(
                               "hyprctl activeworkspace -j |"
                               "jq -c '.id'")
                               ->valueint;
  int num_of_monitors = cJSON_GetArraySize(workspaces_per_monitor);

  if (num_of_monitors > MAX_NUM_OF_MONITORS) {
    perror("found more monitors than supported.");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < num_of_monitors; i++) {
    struct cJSON* workspaces = cJSON_GetArrayItem(workspaces_per_monitor, i);
    int num_of_workspaces = cJSON_GetArraySize(workspaces);
    for (int j = 0; j < num_of_workspaces; j++) {
      struct cJSON* workspace = cJSON_GetArrayItem(workspaces, j);
      struct cJSON* icon = cJSON_GetObjectItem(workspace, "icon");
      int workspace_id = cJSON_GetObjectItem(workspace, "id")->valueint;

      char active_workspace_command[MAX_BUFFER_SIZE] = "";
      snprintf(active_workspace_command, MAX_BUFFER_SIZE,
               "hyprctl monitors -j |"
               "jq -c 'map(.activeWorkspace.id) |"
               "any(. == %d)'",
               workspace_id);
      int is_active_workspace =
          get_hyprctl_output(active_workspace_command)->valueint;

      if (workspace_id == selected_workspace) {
        cJSON_SetValuestring(icon, "󰮯");
      } else if (workspace_id != selected_workspace && is_active_workspace) {
        cJSON_SetValuestring(icon, "");
      } else {
        cJSON_SetValuestring(icon, "󰊠");
      }
    }
  }
  return workspaces_per_monitor;
}
