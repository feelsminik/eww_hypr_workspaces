#ifndef __hypr__h
#define __hypr__h

#include <stddef.h>
#include <stddef.h>
#include <cjson/cJSON.h>

// Create path to hypr socket
char* hypr_socket_path(char*, size_t);

bool is_changing_workspace(char*); 

cJSON* get_hypr_workspaces();

#endif //__hypr__h
