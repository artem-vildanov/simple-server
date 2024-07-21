#include "../include/event_handler.h"

volatile int running = 1;

struct ServerContext* init_server_context(int server_socket, int* command_pipe);
void init_read_fds(struct ServerContext* context);
void await_for_events(struct ServerContext* context);
void handle_command_event(struct ServerContext* context);
void handle_connection_event(struct ServerContext* context);

void handle_events(int server_socket, int* command_pipe) 
{
  struct ServerContext* context = init_server_context(server_socket, command_pipe); 

  while(running) 
  {
    init_read_fds(context);
    await_for_events(context);
    handle_command_event(context);
    handle_connection_event(context);
  }
}

struct ServerContext* init_server_context(int server_socket, int* command_pipe)
{
  struct ServerContext* context = (struct ServerContext *)malloc(sizeof(struct ServerContext));
  context->server_socket = server_socket;
  context->command_pipe = command_pipe;
  context->read_fds = (fd_set *)malloc(sizeof(fd_set));
  context->max_fd = 
    server_socket > command_pipe[0]
    ? server_socket
    : command_pipe[0];

  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    context->client_sockets[i] = 0;
  }

  sem_init(&context->task_semaphore, 0, 0);
  pthread_mutex_init(&context->client_sockets_mutex, NULL);

  for (int i = 0; i < THREAD_POOL_SIZE; i++)
  {
    pthread_create(
      &context->thread_pool[i], 
      NULL, 
      client_handler, 
      context
    );
  }

  return context;
}

void init_read_fds(struct ServerContext* context)
{
  FD_ZERO(context->read_fds);
  FD_SET(context->server_socket, context->read_fds);
  FD_SET(context->command_pipe[0], context->read_fds);

  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    int client_socket = context->client_sockets[i];
    if (client_socket == 0)
      continue;
    FD_SET(client_socket, context->read_fds);
  }
}

void await_for_events(struct ServerContext* context)
{
  int select_result = select(context->max_fd + 1, context->read_fds, NULL, NULL, NULL);

  if (select_result < 0) 
  {
    perror("select");
    exit(EXIT_FAILURE);
  }
}

void handle_command_event(struct ServerContext* context)
{
  if (!FD_ISSET(context->command_pipe[0], context->read_fds))
    return;

  char command[BUFFER_SIZE];
  int bytes_read = read(context->command_pipe[0], command, BUFFER_SIZE);

  if (bytes_read == 0)
    return;

  command[bytes_read] = '\0';
  printf("Received command: %s\n", command);
  
  if (strcmp(command, "exit") == 0)
  {
    printf("Stopping server...\n");
    stop_server(context);
  }
}

void handle_connection_event(struct ServerContext* context)
{
  if (!running)
    return;

  if (!FD_ISSET(context->server_socket, context->read_fds)) 
    return;

  int client_socket = recieve_connect(context);
  if (!add_client_socket(context, client_socket))
  {
    printf("Too many clients connected\n");
    close(client_socket);
  }
}

