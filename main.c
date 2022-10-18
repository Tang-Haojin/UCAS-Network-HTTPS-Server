#include <signal.h>
#include <threads.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/epoll.h>
#include "general.h"

int msgid;
int sockfd;
int epfd;

int main() {
  signal(SIGPIPE, SIG_IGN);
  sockfd = socket_init();
  if (sockfd == -1) {
    return 0;
  }
  msgid = msgget((key_t)1234, IPC_CREAT | 0600);
  if (msgid == -1) {
    return 0;
  }

  thrd_t id[4];
  for (int i = 0; i < 4; i++) {
    thrd_create(&id[i], thread_func, NULL);
  }

  epfd = epoll_create(MAXFD);
  if (epfd == -1) {
    printf("create epoll error\n");
    return 0;
  }

  epoll_add(epfd, sockfd);

  struct epoll_event evs[MAXFD];
  while (1) {
    int n = epoll_wait(epfd, evs, MAXFD, -1);
    if (n == -1) {
      continue;
    } else {
      struct mess m;
      m.type = 1;
      for (int i = 0; i < n; i++) {
        m.c = evs[i].data.fd;
        if (evs[i].events & EPOLLIN) {
          msgsnd(msgid, &m, sizeof(int), 0);
        }
      }
    }
  }
}