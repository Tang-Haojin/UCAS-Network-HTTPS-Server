#include <threads.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include "general.h"

int thread_func(void *args) {
  while (1) {
    struct mess m;
    msgrcv(msgid, &m, sizeof(int), 1, 0);
    int c = m.c;
    if (c == sockfd) {
      struct sockaddr_in caddr;
      int len = sizeof(caddr);
      int cli = accept(sockfd, (struct sockaddr *)&caddr, &len);
      if (cli < 0) {
        continue;
      }
      epoll_add(epfd, cli);
    } else {
      char buff[1024] = {};
      int n = recv(c, buff, 1023, 0);
      if (n <= 0) {
        epoll_del(epfd, c);
        close(c);
        printf("close\n");
        continue;
      }
      char *filename = get_filename(buff);
      if (filename == NULL) {
        send_404status(c);
        epoll_del(epfd, c);
        close(c);
        continue;
      }
      printf("filename: %s\n", filename);

      if (send_httpfile(c, filename) == -1) {
        printf("主动关闭连接\n");
        epoll_del(epfd, c);
        close(c);
        continue;
      }
    }
    epoll_mod(epfd, c);
  }
  return 0;
}