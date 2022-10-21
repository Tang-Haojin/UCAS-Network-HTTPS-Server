#include "general.h"

int socket_init(int port) {
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  Assert(socketfd != -1, "socket create error");
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = inet_addr("0.0.0.0");

  int res = bind(socketfd, (struct sockaddr *)&saddr, sizeof(saddr));
  Assert(res != -1, "bind error");

  res = listen(socketfd, 5);
  Assert(res != -1, "listen error");

  return socketfd;
}
