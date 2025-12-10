#ifndef CLIENT_H
#define CLIENT_H

#include "defs.h"

struct client_state {
  int fd;
  char buffer[BUFFER_SIZE];
  ssize_t buf_len;
};

void *handle_client(void *arg);

#endif



