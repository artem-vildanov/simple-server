#include "../include/client_handler.h"


int get_client_socket(int server_socket);
int find_active_client_socket(int* clients_sockets);
void handle_connect(int client_socket);

int recieve_connect(struct ServerContext* context)
{
  int client_fd = get_client_socket(context->server_socket);
  set_non_blocking(client_fd);
  FD_SET(client_fd, context->read_fds);

  if (client_fd > context->max_fd) 
    context->max_fd = client_fd;

  return client_fd;
}

int get_client_socket(int server_socket) 
{
  int client_fd = accept(
    server_socket, 
    NULL,
    NULL
  );

  if (client_fd < 0) {
    perror("accept failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("New connection, socket fd is %d\n", client_fd);
  return client_fd;
}

int add_client_socket(struct ServerContext* context, int client_socket)
{
  pthread_mutex_lock(&context->client_sockets_mutex);

  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (context->client_sockets[i] != 0)
      continue;

    context->client_sockets[i] = client_socket;
    pthread_mutex_unlock(&context->client_sockets_mutex);
    sem_post(&context->task_semaphore);
    return SUCCESS;
  }

  pthread_mutex_unlock(&context->client_sockets_mutex);
  return FAIL; // очередь клиентов заполнена
}

void *client_handler(void *arg)
{
  struct ServerContext* context = (struct ServerContext *)arg;

  while(running)
  {
    printf("Thread waits for semaphore signal\n");
    sem_wait(&context->task_semaphore);
    printf("Thread awaked, started working\n");

    pthread_mutex_lock(&context->client_sockets_mutex);
    int client_socket = find_active_client_socket(context->client_sockets); 
    pthread_mutex_unlock(&context->client_sockets_mutex);

    if (client_socket == 0)
      continue;

    handle_connect(client_socket);
    close(client_socket);
  }
  return NULL;
}

int find_active_client_socket(int* clients_sockets)
{
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (clients_sockets[i] == 0)
      continue;

    int active_socket = clients_sockets[i];
    clients_sockets[i] = 0;
    return active_socket;
  }

  return FAIL;
}

void handle_connect(int client_socket)
{
  char buffer[BUFFER_SIZE] = {0};
  
  read(client_socket, buffer, BUFFER_SIZE);
  printf("Client request received: %s\n", buffer);

  char *server_response = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello from server!\n";

  write(client_socket, server_response, strlen(server_response));
  printf("Server response sent\n");
}
