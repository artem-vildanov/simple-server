#include "../include/command_handler.h"

pthread_t *create_command_thread(int* command_pipe);
void *handle_commands(void *arg); 
int* create_command_pipe(); 
void finish_threads(sem_t* task_semaphore, pthread_t* thread_pool);

struct Command* create_command_handler()
{
  struct Command* command = (struct Command *)malloc(sizeof(struct Command));
  command->command_pipe = create_command_pipe();
  command->command_thread = create_command_thread(command->command_pipe);
  return command;
}

pthread_t *create_command_thread(int* command_pipe) 
{
  pthread_t* command_thread = malloc(sizeof(pthread_t));

  if (pthread_create(command_thread, NULL, handle_commands, command_pipe) != 0) 
  {
    perror("Failed to create command handler thread");
    exit(EXIT_FAILURE);
  }

  return command_thread;
}

void *handle_commands(void *arg) 
{
  int *command_pipe = (int *)arg;
  char command[BUFFER_SIZE];

  while (1) 
  {
    fflush(stdin);
    fgets(command, sizeof(command), stdin);

    // Удаляем символ новой строки
    command[strcspn(command, "\n")] = '\0';

    if (strcmp(command, "exit") == 0) {
      write(command_pipe[1], "exit", 4);
      break;
    }
  }

  return NULL;
}

int* create_command_pipe() 
{
  int* command_pipe = malloc(2 * sizeof(int));
  if (pipe(command_pipe) != 0) {
    perror("pipe failed");
    free(command_pipe);
    exit(EXIT_FAILURE);
  }

  return command_pipe;
}

void stop_server(struct ServerContext* context)
{
  running = 0;
  finish_threads(&context->task_semaphore, context->thread_pool); 
  sem_destroy(&context->task_semaphore);
  pthread_mutex_destroy(&context->client_sockets_mutex);
  close(context->server_socket);
  free(context->read_fds);
  free(context);
}

void finish_threads(sem_t* task_semaphore, pthread_t* thread_pool)
{
  for (int i = 0; i < THREAD_POOL_SIZE; i++)
    sem_post(task_semaphore);

  for (int i = 0; i < THREAD_POOL_SIZE; i++)
    pthread_join(thread_pool[i], NULL);
}
