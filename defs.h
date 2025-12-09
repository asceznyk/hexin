#ifndef DEFS_H
#define DEFS_H

#define PORT 6969
#define BUFFER_SIZE 8192
#define NUM_CONCURRENT 10

#define STATIC_LOCATION "/var/www/html"

struct client_state {
  int fd;
  char buffer[BUFFER_SIZE];
  ssize_t buf_len;
};

#endif

