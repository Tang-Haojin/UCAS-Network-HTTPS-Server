#include "general.h"

int send_file(SSL *ssl, int socketfd, char *filename) {
  if (filename == NULL || socketfd < 0) {
    SSL_send(ssl, socketfd, "err", 3);
    return -1;
  }

  char path[128] = PATH;
  strcat(path, filename);
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    send_404_status(ssl, socketfd);
    return -1;
  }

  long size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  char head_buff[512] = "HTTP/1.1 200 OK\r\n"
                        "Accept-Ranges: bytes\r\n"
                        "Server: SugarCake\r\n";
  sprintf(head_buff + strlen(head_buff), "Content-Length: %ld\r\n", size);
  strcat(head_buff, "\r\n");
  SSL_send(ssl, socketfd, head_buff, strlen(head_buff));
  printf("send file:\n%s\n", head_buff);

  int num = 0;
  char data[1024] = {};
  while ((num = read(fd, data, 1024)) > 0) {
    SSL_send(ssl, socketfd, data, num);
  }

  close(fd);
  return 0;
}
