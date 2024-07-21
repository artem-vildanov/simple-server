#ifndef SOCKET_FD_UTILS_H
#define SOCKET_FD_UTILS_H

#include <fcntl.h>  // Для fcntl
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void set_non_blocking(int socket_fd); 

#endif //SOCKET_FD_UTILS_H
