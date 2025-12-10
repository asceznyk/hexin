#include <stdio.h>
#include <string.h>

int ends_with_double_crlf(const char *buf, size_t len) {
  if (len < 4) return 0;
  return buf[len-4] == '\r' &&
  buf[len-3] == '\n' &&
  buf[len-2] == '\r' &&
  buf[len-1] == '\n';
}

void print_escaped(const char *buf, ssize_t n) {
  if(n) printf("print_escaped: \t");
  for (ssize_t i = 0; i < n; i++) {
    unsigned char c = buf[i];
    if (c == '\r') printf("\\r");
    else if (c == '\n') printf("\\n");
    else if (c == '\t') printf("\\t");
    else if (c < 32 || c > 126)
      printf("\\x%02X", c);
    else
      printf("%c", c);
  }
  printf("\n");
}


