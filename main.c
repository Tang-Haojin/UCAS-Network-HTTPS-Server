#include "general.h"

int msgid;
int socket80fd;
int socket443fd;
int epfd;

int main() {
  Assert(sizeof(my_epoll_data_t) == sizeof(epoll_data_t), "size of epoll_data_t unusual");
  signal(SIGPIPE, SIG_IGN);

  socket80fd = socket_init(80);
  socket443fd = socket_init(443);

  msgid = msgget((key_t)1234, IPC_CREAT | 0600);
  Assert(msgid != -1, "msgget error");

  thrd_t id[4];
  for (int i = 0; i < 4; i++) {
    thrd_create(&id[i], thread_func, NULL);
  }

  epfd = epoll_create(MAXFD);
  Assert(epfd != -1, "create epoll error");

  epoll_add(epfd, socket80fd, 0);
  epoll_add(epfd, socket443fd, 0);

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