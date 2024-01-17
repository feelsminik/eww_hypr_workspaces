#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

extern int connect(int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);

int main(void) {
  struct sockaddr_un server_addr;
  char* hypr_signature = getenv("HYPRLAND_INSTANCE_SIGNATURE");
  char server_path[128] = "";
  snprintf(server_path, sizeof(server_path), "/tmp/hypr/%s/.socket2.sock",
           hypr_signature);

  /* memset(&server_addr, 0, sizeof(server_addr)); */
  /* server_addr.sun_family = AF_UNIX; */
  /* strcpy(server_addr.sun_path, server_path); */

  printf("%s", server_path);

  return 0;
}
