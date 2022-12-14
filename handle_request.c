#include "general.h"

int handle_request(struct mess *m, struct request_fields *request) {
  if (m->data->parent_fd == socket80fd) {
    send_301_status(m->data->ssl, m->data->fd, request->Host, request->filename);
    return -1;
  }

  if (request->filename == NULL) {
    send_404_status(m->data->ssl, m->data->fd);
    return -1;
  }

  printf("filename: %s\n", request->filename);

  if (request->Range) {
    if (send_206_file(m->data->ssl, m->data->fd, request->Range, request->filename) == -1) {
      return -1;
    }
    return 0;
  }

  if (send_file(m->data->ssl, m->data->fd, request->filename) == -1) {
    return -1;
  }
  return 0;
}
