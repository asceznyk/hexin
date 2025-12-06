#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 6969
#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
  int client_fd = *(int*)arg;
  free(arg);
  char buffer[BUFFER_SIZE];
  while (true) {
    ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
    if (n <= 0) break;
    printf("recv -> message = %s\n", buffer);
    send(client_fd, buffer, n, 0);
  }
  close(client_fd);
}

int main() {
  int server_fd;
  int opt = 1;
  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("failed to create socket!\n");
    exit(EXIT_FAILURE);
  }
  if(
    setsockopt(
      server_fd,
      SOL_SOCKET,
      SO_REUSEADDR | SO_REUSEPORT,
      &opt,
      sizeof(opt)
    ) < 0
  ) {
    perror("failed to set socket options!\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  if (
    bind(
      server_fd,
      (struct sockaddr*)&server_addr,
      sizeof(server_addr)
    ) < 0
  ) {
    perror("failed bind to socket address\n");
    exit(EXIT_FAILURE);
  }
  if(listen(server_fd, 3) < 0) {
    perror("failed to listen!\n");
    exit(EXIT_FAILURE);
  }
  printf("server is listening on PORT %d\n", PORT);
  while (true) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int *client_fd = malloc(sizeof(int));
    if(
      *client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len) < 0
    ) {
      perror("cound't accept connection request!");
      continue;
    };
    pthread_t tid;
    pthread_create(&tid, NULL, &handle_client, client_fd);
    pthread_detach(tid);
  }
  close(server_fd);
  return 0;
}

