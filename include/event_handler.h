#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <pthread.h>
#include "constants.h"
#include "socket_fd_utils.h"
#include "client_handler.h"
#include "command_handler.h"

void handle_events(int server_socket, int* command_pipe); 

#endif // EVENT_HANDLER_H
