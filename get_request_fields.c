#include "general.h"

#define CAS_req_fields(name) (fields.name = strcmp(key, #name) ? fields.name : value)

inline char *strip_space(char *str) {
  for (; *str == ' '; str++);
  char *tail = str + strlen(str) - 1;
  for (; *tail == ' '; tail--)
    *tail = 0;
  return str;
}

struct request_fields get_request_fields(char buff[]) {
  struct request_fields fields = {};
  char *ptr = NULL;
  char *s = strtok_r(buff, " ", &ptr);
  if (s == NULL) {
    printf("Request message error\n");
    return fields;
  }
  printf("Request method: %s\n", s);
  s = strtok_r(NULL, " ", &ptr);
  if (s == NULL) {
    printf("Request message no resources name\n");
    return fields;
  }
  strtok_r(NULL, "\n", &ptr);
  fields.filename = strcmp(s, "/") ? s : "/index.html";

  do {
    char *key = strip_space(strtok_r(NULL, ":", &ptr));
    char *value = strip_space(strtok_r(NULL, "\r", &ptr));
    ptr++;
    apply_all_request(CAS_req_fields);
  } while (*ptr != '\r' && *ptr != 0);
  return fields;
}
