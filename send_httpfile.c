#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include "general.h"

int send_httpfile(int c, char *filename) {
  if (filename == NULL || c < 0) {
    send(c, "err", 3, 0);
    return -1;
  }

  char path[128] = PATH;
  strcat(path, filename);
  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    send_404status(c);
    return -1;
  }

  int size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  char head_buff[512] = "HTTP/1.1 200 OK\r\n";
  strcat(head_buff, "Server: SugarCake\r\n");
  sprintf(head_buff + strlen(head_buff), "Content-Length: %d\r\n", size);
  strcat(head_buff, "\r\n");
  send(c, head_buff, strlen(head_buff), 0);
  printf("send file:\n%s\n", head_buff);

  int num = 0;
  char data[1024] = {};
  while ((num = read(fd, data, 1024)) > 0) {
    send(c, data, num, 0);
  }

  close(fd);
  return 0;
}