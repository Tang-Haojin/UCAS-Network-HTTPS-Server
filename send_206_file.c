#include "general.h"

int send_206_file(SSL *ssl, int socketfd, char *range, char *filename) {
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
  long start = 0, end = size - 1;
  if (sscanf(range, "bytes%*[= ]%*[- ]%ld", &end) == 0) {
    sscanf(range, "bytes%*[= ]%ld%*[- ]%ld", &start, &end);
  }

  char head_buff[512];
  if (start > end || start > size - 1) {
    strcpy(head_buff, "HTTP/1.1 416 Requested Range Not Satisfiable\r\n"
                      "Server: SugarCake\r\n");
    sprintf(head_buff + strlen(head_buff), "Content-Length: 0\r\n");
    sprintf(head_buff + strlen(head_buff), "Content-Range: bytes */%ld\r\n", size);
    strcat(head_buff, "\r\n");
    SSL_send(ssl, socketfd, head_buff, strlen(head_buff));
    printf("send file:\n%s\n", head_buff);
    close(fd);
    return -1;
  }
  if (end > size - 1)
    end = size - 1;
  lseek(fd, start, SEEK_SET);

  strcpy(head_buff, "HTTP/1.1 206 Partial Content\r\n"
                    "Server: SugarCake\r\n");
  sprintf(head_buff + strlen(head_buff), "Content-Length: %ld\r\n", end - start + 1);
  sprintf(head_buff + strlen(head_buff), "Content-Range: bytes %ld-%ld/%ld\r\n", start, end, size);
  strcat(head_buff, "\r\n");
  SSL_send(ssl, socketfd, head_buff, strlen(head_buff));
  printf("send file:\n%s\n", head_buff);

  ssize_t pending_size = end - start + 1;
  ssize_t num = 0;
  char data[1024] = {};
  while (pending_size > 0) {
    num = read(fd, data, pending_size < 1024 ? pending_size : 1024);
    SSL_send(ssl, socketfd, data, num);
    pending_size -= num;
  }

  close(fd);
  return 0;
}
