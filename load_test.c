#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_MSGS 5

int PORT = 5000;
char *IP = "127.0.0.1";

void* client_thread(void *arg) {
  int id = *(int*)arg;
  free(arg);
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = inet_addr(IP);
  if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    printf("Client %d: connect failed\n", id);
    close(sock);
    return NULL;
  }
  printf("Client %d connected.\n", id);
  char msg[BUFFER_SIZE];
  char recv_buf[BUFFER_SIZE];
  for (int i = 0; i < MAX_MSGS; i++) {
    snprintf(msg, BUFFER_SIZE, "Hello from client %d (msg %d)", id, i);
    send(sock, msg, strlen(msg), 0);
    ssize_t n = recv(sock, recv_buf, BUFFER_SIZE-1, 0);
    if (n <= 0) break;
    recv_buf[n] = '\0';
    printf("Client %d got: %s\n", id, recv_buf);
    usleep(100000); // 100ms
  }
  close(sock);
  return NULL;
}

int main(int argc, char *argv[]) {
  int N = 5;
  if (argc == 2) N = atoi(argv[1]);
  pthread_t threads[N];
  for (int i = 0; i < N; i++) {
    int *id = malloc(sizeof(int));
    *id = i;
    pthread_create(&threads[i], NULL, client_thread, id);
  }
  for (int i = 0; i < N; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}

