#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
  printf("PORT, BUFFER_SIZE = %d, %d\n", PORT, BUFFER_SIZE);
  int server_fd;
  struct sockaddr_in address;
  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    printf("failed to create socket!\n");
    exit(EXIT_FAILURE);
  }
  if(
    setsockopt(
      server_fd,
      SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
      &opt,
      sizeof(opt)
    )
  ) {
    exit(EXIT_FAILURE);
  }
  printf("server_fd = %d\n", server_fd);
  return 0;
}

