#include "../include/server_setup.h"


int set_server_socket();
struct sockaddr_in* set_server_address();
void bind_socket_to_address(int server_socket, struct sockaddr_in* address);
void listen_connections(int server_socket);
void print_server_address(int server_socket); 
void reuse_address(int server_socket);

// return server_socket
int setup_server() {
  struct sockaddr_in* server_address = set_server_address();
  int server_socket = set_server_socket();
  reuse_address(server_socket);
  set_non_blocking(server_socket);
  bind_socket_to_address(server_socket, server_address);
  print_server_address(server_socket);  
  listen_connections(server_socket);
  return server_socket;
}

int set_server_socket() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  return server_socket;
}

void reuse_address(int server_socket)
{
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    return;
  }
}

struct sockaddr_in* set_server_address() {
  struct sockaddr_in* address = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
  address->sin_family = AF_INET;
  address->sin_addr.s_addr = INADDR_ANY;
  address->sin_port = htons(PORT);
  return address;
}

void bind_socket_to_address(int server_socket, struct sockaddr_in* server_address) {
  int bind_result = bind(
    server_socket, 
    (struct sockaddr *)server_address, 
    sizeof(*server_address)
  );  
  if (bind_result < 0) {
    perror("bind failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }
}

void print_server_address(int server_socket) 
{
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);

  if (getsockname(server_socket, (struct sockaddr *)&addr, &addr_len) < 0) {
    perror("getsockname");
    return;
  }

  char ip_str[INET_ADDRSTRLEN];
  if (inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str)) == NULL) {
    perror("inet_ntop");
    return;
  }

  printf("Server listening on %s:%d\n", ip_str, ntohs(addr.sin_port));
}

void listen_connections(int server_socket) 
{
  if (listen(server_socket, 3) < 0) {
    perror("listen");
    close(server_socket);
    exit(EXIT_FAILURE);
  }
}
