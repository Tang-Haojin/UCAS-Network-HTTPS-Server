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
#include <sys/epoll.h>
#include <sys/msg.h>
#include <sys/socket.h>

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

extern int msgid;
extern int socket80fd;
extern int socket443fd;
extern int epfd;

int socket_init(int port);
int thread_func(void *args);
int send_httpfile(int c, char *filename);
int send_404status(int c);
char *get_filename(char buff[]);
void epoll_add(int epfd, int fd, uint32_t parent_fd);
void epoll_del(int epfd, int fd);
void epoll_mod(int epfd, int fd, uint32_t parent_fd);

struct mess {
  long type;
  int c;
};

typedef struct my_epoll_data {
  int fd;
  uint32_t parent_fd;
} my_epoll_data_t;

struct my_epoll_event {
  uint32_t events;	/* Epoll events */
  my_epoll_data_t data;	/* User data variable */
} __EPOLL_PACKED;

#endif
