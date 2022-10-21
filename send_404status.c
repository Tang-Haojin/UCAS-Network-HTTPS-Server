#include "general.h"

int send_404status(SSL *ssl, int socketfd) {
  char path[128] = PATH "/404.html";
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    SSL_send(ssl, socketfd, "404", 3);
    return 0;
  }

  int size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  char head_buff[512] = "HTTP/1.1 404 Not Found\r\n";
  strcat(head_buff, "Server: SugarCake\r\n");
  sprintf(head_buff + strlen(head_buff), "Content-Length: %d\r\n", size);
  strcat(head_buff, "\r\n");
  SSL_send(ssl, socketfd, head_buff, strlen(head_buff));

  char data[1024] = {};
  int num = 0;
  while ((num = read(fd, data, 1024)) > 0) {
    SSL_send(ssl, socketfd, data, num);
  }

  close(fd);
  return 0;
}
