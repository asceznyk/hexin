#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "http_response.h"

char *response_not_found =
  "HTTP/1.1 404 Not Found\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 68\r\n"
  "Connection: close\r\n"
  "\r\n"
  "<html><body><h1>404 Not Found</h1><p>The page is missing.</p></body></html>";

char *get_http_method(char *buffer) {
  char *end = skip_leading_ws(buffer);
  while (*end && *end != ' ') end++;
  size_t len = end - buffer;
  char *method = malloc(len + 1);
  if(!method) return NULL;
  memcpy(method, buffer, len);
  method[len] = '\0';
  return method;
}

char *get_file_path_url(char *buffer, char *method) {
  char *start = skip_leading_ws(buffer);
  start += strlen(method)+1;
  char *end = start;
  while(*end && *end != ' ') end++;
  size_t len = end - start;
  char *loc = malloc(len + 1);
  memcpy(loc, start, len);
  loc[len] = '\0';
  return loc;
}

char *http_response(struct client_state *state) {
  if(state->buf_len <= 0) return "";
  char *method = get_http_method(state->buffer);
  if (strcmp(method, "GET")) {
    return NULL; //TODO proxy/HEAD, parse other methods!
  }
  if (strcmp(STATIC_LOCATION, "")) {
    char *path = get_file_path_url(state->buffer, method);
    char *loc = str_concat(STATIC_LOCATION, path);
    free(path);
    free(loc);
  }
  free(method);
  return response_not_found;
}


