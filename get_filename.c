#include "general.h"

char *get_filename(char buff[]) {
  char *ptr = NULL;
  char *s = strtok_r(buff, " ", &ptr);
  if (s == NULL) {
    printf("请求报文错误\n");
    return NULL;
  }
  printf("请求方法: %s\n", s);
  s = strtok_r(NULL, " ", &ptr);
  if (s == NULL) {
    printf("请求报文 无资源名字\n");
    return NULL;
  }
  if (strcmp(s, "/") == 0) {
    return "/index.html";
  }
  return s;
}