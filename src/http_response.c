#include <stdlib.h>

#include "http_response.h"

char *response_not_found =
  "HTTP/1.1 404 Not Found\r\n"
  "Content-Type: text/html\r\n"
  "Content-Length: 68\r\n"
  "Connection: close\r\n"
  "\r\n"
  "<html><body><h1>404 Not Found</h1><p>The page is missing.</p></body></html>";

char *http_response(struct client_state *state) {
  if(state->buf_len <= 0) return "";
  /*if (STATIC_LOCATION != "") {
    char *full_path = STATIC_LOCATION + ;
    if
  }*/
  return response_not_found;
}


