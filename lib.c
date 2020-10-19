#include <stdio.h>

void log_result(char* func_name, int count) {
  printf("function `%s` ran  %d dynamic mul instructions\n", func_name, count);
}
