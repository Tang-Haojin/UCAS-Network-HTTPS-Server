#ifndef __GENERAL_H__
#define __GENERAL_H__

#define MAXFD 4
#define PATH "/home/mandelbrot/http_server"

extern int msgid;
extern int sockfd;
extern int epfd;

int socket_init();
int thread_func(void *args);
int send_httpfile(int c, char *filename);
int send_404status(int c);
char *get_filename(char buff[]);
void epoll_add(int epfd, int fd);
void epoll_del(int epfd, int fd);
void epoll_mod(int epfd, int fd);

struct mess {
  long type;
  int c;
};

#endif
