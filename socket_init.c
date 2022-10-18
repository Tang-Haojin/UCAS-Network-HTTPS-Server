#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>

int socket_init() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    return -1;
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(80);
  saddr.sin_addr.s_addr = inet_addr("0.0.0.0");

  int res = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
  if (res == -1) {
    printf("bind err\n");
    return -1;
  }
  res = listen(sockfd, 5);
  if (res == -1)
    return -1;
  return sockfd;
}
