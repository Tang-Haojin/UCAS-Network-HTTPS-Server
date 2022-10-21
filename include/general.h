#ifndef __GENERAL_H__
#define __GENERAL_H__

#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <sys/epoll.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/time.h>

#define MAXFD 4
#define PATH "/home/mandelbrot/http_server"
#define Assert(cond, ...) \
  do { \
    if (!(cond)) { \
      fflush(stdout); \
      fprintf(stderr, "\33[1;31m"); \
      fprintf(stderr, __VA_ARGS__); \
      fprintf(stderr, "\33[0m\n"); \
      assert(cond); \
    } \
  } while (0)

#define SSL_recv(ssl, __fd, __buf, __n) \
  ((ssl) ? SSL_read(ssl, __buf, __n) : recv(__fd, __buf, __n, 0))

#define SSL_send(ssl, __fd, __buf, __n) \
  ((ssl) ? SSL_write(ssl, __buf, __n) : send(__fd, __buf, __n, 0))

typedef struct my_epoll_data {
  int fd;
  int parent_fd;
  SSL *ssl;
} my_epoll_data_t;

struct mess {
  long type;
  my_epoll_data_t *data;
};

extern int msgid;
extern int socket80fd;
extern int socket443fd;
extern int epfd;
extern SSL_CTX *ctx;

void initSSL(const char *cacert, const char *key);
int socket_init(int port);
int thread_func(void *args);
int send_file(SSL *ssl, int socketfd, char *filename);
int send_404status(SSL *ssl, int socketfd);
char *get_filename(char buff[]);
struct my_epoll_data *epoll_add(int fd, int parent_fd, SSL *ssl);
void epoll_del(struct my_epoll_data *data);
void epoll_mod(struct my_epoll_data *data);

#endif
