#ifndef __hypr__h
#define __hypr__h

#include <stddef.h>

// Create path to hypr socket
char* hypr_socket_path(char*, size_t);

bool is_changing_workspace(char*); 

#endif //__hypr__h
