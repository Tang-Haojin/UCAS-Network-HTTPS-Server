#include "general.h"

struct my_epoll_data *epoll_add(int fd, int parent_fd, SSL *ssl) {
  struct epoll_event ev;
  struct my_epoll_data *data = ev.data.ptr = calloc(1, sizeof(struct my_epoll_data));
  data->fd = fd;
  data->parent_fd = parent_fd;
  data->ssl = ssl;
  ev.events = EPOLLIN | EPOLLONESHOT;

  Assert(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) != -1, "epoll add error");
  return data;
}

void epoll_del(struct my_epoll_data *data) {
  if (epoll_ctl(epfd, EPOLL_CTL_DEL, data->fd, NULL) == -1) {
    printf("epoll del error\n");
  }
  close(data->fd);
  free(data);
}

void epoll_mod(struct my_epoll_data *data) {
  struct epoll_event ev;
  ev.data.ptr = data;
  ev.events = EPOLLIN | EPOLLONESHOT;
  printf("epoll_mod:\n\tssl: %p\n", data->ssl);

  Assert(epoll_ctl(epfd, EPOLL_CTL_MOD, data->fd, &ev) != -1, "epoll mod error");
}
