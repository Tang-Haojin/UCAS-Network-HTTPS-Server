#include "general.h"

int send_301_status(SSL *ssl, int socketfd, char *host, char *filename) {
  char head_buff[512] = "HTTP/1.1 301 Moved Permanently\r\n"
                        "Server: SugarCake\r\n"
                        "Content-Length: 0\r\n";
  sprintf(head_buff + strlen(head_buff), "Location: https://%s%s\r\n", host, filename);
  strcat(head_buff, "\r\n");
  SSL_send(ssl, socketfd, head_buff, strlen(head_buff));
  printf("send file:\n%s\n", head_buff);
  return 0;
}
