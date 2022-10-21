#include "general.h"

void epoll_add(int epfd, int fd, uint32_t parent_fd) {
  struct my_epoll_event ev;
  ev.data.fd = fd;
  ev.data.parent_fd = parent_fd;
  ev.events = EPOLLIN | EPOLLONESHOT;

  Assert(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, (struct epoll_event *)&ev) != -1, "epoll add error");
}

void epoll_del(int epfd, int fd) {
  if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
    printf("epoll del error\n");
  }
}

void epoll_mod(int epfd, int fd, uint32_t parent_fd) {
  struct my_epoll_event ev;
  ev.data.fd = fd;
  ev.data.parent_fd = parent_fd;
  ev.events = EPOLLIN | EPOLLONESHOT;

  Assert(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, (struct epoll_event *)&ev) != -1, "epoll mod error");
}
