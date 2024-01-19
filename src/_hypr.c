#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
