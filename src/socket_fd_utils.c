#include "../include/socket_fd_utils.h"

void set_non_blocking(int socket_fd) 
{
  int flags = fcntl(socket_fd, F_GETFL, 0);
  if (flags < 0) 
  {
    perror("fcntl");
    exit(EXIT_FAILURE);
  }
  int set_non_blocking_result = fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
  if (set_non_blocking_result < 0) 
  {
    perror("set_non_blocking");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }
}
