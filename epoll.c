#include <stddef.h>
#include <sys/epoll.h>
#include <stdio.h>

void epoll_add(int epfd, int fd) {
  struct epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLIN | EPOLLONESHOT;

  if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
    printf("epoll add error\n");
  }
}

void epoll_del(int epfd, int fd) {
  if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
    printf("epoll del error\n");
  }
}

void epoll_mod(int epfd, int fd) {
  struct epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLIN | EPOLLONESHOT;
  if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1) {
    printf("epoll mod error\n");
  }
}
