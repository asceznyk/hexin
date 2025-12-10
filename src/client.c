#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "defs.h"
#include "utils.h"
#include "client.h"

void append_to_state(
  struct client_state *state,
  const char *tmp_buf,
  ssize_t n
) {
  if (state->buf_len+n > BUFFER_SIZE) {
    n = BUFFER_SIZE - state->buf_len;
  }
  memcpy(state->buffer + state->buf_len, tmp_buf, n);
  state->buf_len += n;
}

void check_and_empty_state(struct client_state *state) {
  if (!ends_with_double_crlf(state->buffer, state->buf_len)) return;
  memset(state->buffer, 0, state->buf_len);
  state->buf_len = 0;
}

void process_request(struct client_state *state) {
  char *client_buf = state->buffer;
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
