#include "../include/server_setup.h"
#include "../include/command_handler.h"
#include "../include/event_handler.h"
#include <pthread.h>


int main()
{
  int server_socket = setup_server(); 
  struct Command* command = create_command_handler();
  handle_events(server_socket, command->command_pipe);

  pthread_join(*command->command_thread, NULL);
  return 0;
}
