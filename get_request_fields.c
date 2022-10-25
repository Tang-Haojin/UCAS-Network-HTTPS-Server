#include "general.h"

#define CAS_req_fields(name) (fields.name = strcmp(key, #name) ? fields.name : value)

struct request_fields get_request_fields(char buff[]) {
  struct request_fields fields = {};
  char *ptr = NULL;
  char *s = strtok_r(buff, " ", &ptr);
  if (s == NULL) {
    printf("请求报文错误\n");
    return fields;
  }
  printf("请求方法: %s\n", s);
  s = strtok_r(NULL, " ", &ptr);
  if (s == NULL) {
    printf("请求报文 无资源名字\n");
    return fields;
  }
  strtok_r(NULL, "\n", &ptr);
  fields.filename = strcmp(s, "/") ? s : "/index.html";

  do {
    char *key = strtok_r(NULL, ":", &ptr);
    ptr++;
    char *value = strtok_r(NULL, "\r", &ptr);
    ptr++;
    apply_all_request(CAS_req_fields);
  } while (*ptr != '\r' && *ptr != 0);
  return fields;
}
