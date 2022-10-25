#include "general.h"
#include <errno.h>

void close_connection(struct my_epoll_data *data) {
  if (data->ssl) {
    SSL_shutdown(data->ssl);
    SSL_free(data->ssl);
  }
  epoll_del(data);
}

int thread_func(void *args) {
  while (1) {
    struct mess m;
    msgrcv(msgid, &m, sizeof(struct my_epoll_data *), 1, 0);
    int socketfd = m.data->fd;
    int parentfd = m.data->parent_fd;
    if (socketfd == socket80fd || socketfd == socket443fd) {
      int cli = accept(socketfd, NULL, NULL);
      if (cli < 0) {
        printf("error! errno = %d\n", errno);
        epoll_mod(m.data);
        continue;
      }
      epoll_add(cli, socketfd, NULL);
    } else {
      char buff[0x1000] = {};

      if (!m.data->ssl && parentfd == socket443fd) {
        Assert((m.data->ssl = SSL_new(ctx)) != NULL, "ssl new error");
        SSL_set_accept_state(m.data->ssl);
        SSL_set_fd(m.data->ssl, socketfd);
        if (SSL_accept(m.data->ssl) != 1) {
          close_connection(m.data);
          printf("close because ssl error\n");
          continue;
        }
      }

      int n = SSL_recv(m.data->ssl, socketfd, buff, 0x1000 - 1);
      Assert(n < 0x1000 - 1, "buffer is too small");
      if (n <= 0) {
        close_connection(m.data);
        printf("close because n <= 0\n");
        continue;
      }

      struct request_fields request = get_request_fields(buff);
      if (handle_request(&m, &request)) {
        continue;
      }
    }
    epoll_mod(m.data);
  }
  return 0;
}
