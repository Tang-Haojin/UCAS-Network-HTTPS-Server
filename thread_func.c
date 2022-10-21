#include "general.h"

int thread_func(void *args) {
  while (1) {
    struct mess m;
    msgrcv(msgid, &m, sizeof(int), 1, 0);
    int socketfd = m.c;
    if (socketfd == socket80fd || socketfd == socket443fd) {
      struct sockaddr saddr;
      socklen_t len;
      int cli = accept(socketfd, &saddr, &len);
      if (cli < 0)
        continue;
      epoll_add(epfd, cli, socketfd);
    } else {
      char buff[0x1000] = {};
      int n = recv(socketfd, buff, 0x1000 - 1, 0);
      if (n <= 0) {
        epoll_del(epfd, socketfd);
        close(socketfd);
        printf("close\n");
        continue;
      }
      char *filename = get_filename(buff);
      if (filename == NULL) {
        send_404status(socketfd);
        epoll_del(epfd, socketfd);
        close(socketfd);
        continue;
      }
      printf("filename: %s\n", filename);

      if (send_httpfile(socketfd, filename) == -1) {
        printf("主动关闭连接\n");
        epoll_del(epfd, socketfd);
        close(socketfd);
        continue;
      }
    }
    epoll_mod(epfd, socketfd, 0);
  }
  return 0;
}
