#ifndef SERVER_SETUP_H
#define SERVER_SETUP_H

#include "constants.h"
#include "socket_fd_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>  // Для fcntl

int setup_server();

#endif // SERVER_SETUP_H
