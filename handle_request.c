#include "general.h"

int handle_request(struct mess *m, struct request_fields *request) {
  if (m->data->parent_fd == socket80fd) {
    send_301status(m->data->ssl, m->data->fd, request->Host, request->filename);
    return -1;
  }
  if (request->filename == NULL) {
    send_404status(m->data->ssl, m->data->fd);
    close_connection(m->data);
    return -1;
  }
  printf("filename: %s\n", request->filename);

  if (send_file(m->data->ssl, m->data->fd, request->filename) == -1) {
    close_connection(m->data);
    printf("主动关闭连接\n");
    return -1;
  }

  return 0;
}
