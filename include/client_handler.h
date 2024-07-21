#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <semaphore.h>
#include <pthread.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include "constants.h"
#include "socket_fd_utils.h"

struct ServerContext
{
  fd_set* read_fds;
  int max_fd;
  int server_socket;
  int* command_pipe;
  int client_sockets[MAX_CLIENTS];
  pthread_t thread_pool[THREAD_POOL_SIZE];
  sem_t task_semaphore;
  pthread_mutex_t client_sockets_mutex;
};

extern volatile int running;

int recieve_connect(struct ServerContext* context);
int add_client_socket(struct ServerContext* context, int client_socket);
void *client_handler(void *arg);

#endif // CLIENT_HANDLER_H
