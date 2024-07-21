#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "constants.h"
#include <string.h>
#include <unistd.h>
#include "client_handler.h"

struct Command
{
  int* command_pipe;
  pthread_t* command_thread;
};

extern volatile int running;
struct Command* create_command_handler();
void stop_server(struct ServerContext* context);

#endif // COMMAND_HANDLER_H
