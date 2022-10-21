#include "general.h"

int msgid;
int socket80fd;
int socket443fd;
int epfd;
SSL_CTX *ctx = NULL;

int main() {
  signal(SIGPIPE, SIG_IGN);

  initSSL(PATH "/.keys/cert.pem", PATH "/.keys/privkey.pem");

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

  epoll_add(socket80fd, 0, NULL);
  epoll_add(socket443fd, 0, NULL);

  struct epoll_event evs[MAXFD];

  while (1) {
    int n = epoll_wait(epfd, evs, MAXFD, -1);
    if (n == -1) {
      continue;
    } else {
      struct mess m;
      m.type = 1;
      for (int i = 0; i < n; i++) {
        m.data = evs[i].data.ptr;
        if (evs[i].events & EPOLLIN) {
          msgsnd(msgid, &m, sizeof(struct my_epoll_data *), 0);
        }
      }
    }
  }
}