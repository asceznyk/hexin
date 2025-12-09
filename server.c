#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "defs.h"
#include "utils.h"

void process_request(struct client_state* state) {
  char* client_buf = state->buffer;
  ssize_t n = state->buf_len;
  if(!ends_with_double_crlf(client_buf, n)) return;
  for (ssize_t i = 0; i < n; i++) {
    unsigned char c = client_buf[i];
    printf("%c", c);
  }
  printf("\n");
}

void *handle_client(void *arg) {
  struct client_state *state = arg;
  int client_fd = state->fd;
  char tmp_buf[BUFFER_SIZE];
  while(true) {
    ssize_t bytes_read = recv(client_fd, tmp_buf, BUFFER_SIZE, 0);
    if (bytes_read <= 0) continue;
    print_escaped(tmp_buf, bytes_read);
    append_to_state(state, tmp_buf, bytes_read);
    process_request(state);
    const char *response =
      "HTTP/1.1 200 OK\r\n"
      "Content-Length: 6\r\n"
      "Content-Type: text/plain\r\n"
      "Connection: keep-alive\r\n"
      "\r\n"
      "Hello\n";
    if (ends_with_double_crlf(state->buffer, state->buf_len)) {
      send(client_fd, response, strlen(response), 0);
      check_and_empty_state(state);
    }
  }
  free(state);
  close(client_fd);
  printf("connection closed!\n");
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
  if(listen(server_fd, NUM_CONCURRENT) < 0) {
    perror("failed to listen!\n");
    exit(EXIT_FAILURE);
  }
  printf("server is listening on PORT %d\n", PORT);
  printf("static dir = %s\n", STATIC_LOCATION);
  while (true) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int client_fd;
    if((
      client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len)
    ) < 0) {
      perror("cound't accept connection request!");
      continue;
    }
    struct client_state *state = malloc(sizeof(*state));
    state->fd = client_fd;
    state->buf_len = 0;
    pthread_t tid;
    printf("client is connected!\n");
    pthread_create(&tid, NULL, handle_client, state);
    pthread_detach(tid);
  }
  close(server_fd);
  return 0;
}

