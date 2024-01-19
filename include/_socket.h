#ifndef __socket__h
#define __socket__h

#include <stddef.h>

// Takes path to socket and returns sock_fd
int create_socket(char*);

void close_socket(int);

void read_from_socket(int, char*, size_t);

#endif //__socket__h
