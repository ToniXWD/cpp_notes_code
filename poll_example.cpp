#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TIMEOUT 5000 // 设置超时时间为5秒（5000毫秒）

int main(void) {
  struct pollfd fds[1];
  int ret;

  // 监控标准输入
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN; // 监控读取事件

  while (true) {
    // 等待事件发生或超时
    ret = poll(fds, 1, TIMEOUT);

    if (ret == -1) {
      perror("poll");
      return EXIT_FAILURE;
    }

    if (!ret) {
      printf("%d milliseconds elapsed.\n", TIMEOUT);
      continue;
    }

    if (fds[0].revents & POLLIN) {
      char buf[1024];
      ssize_t len = read(STDIN_FILENO, buf, sizeof(buf) - 1);
      if (len > 0) {
        buf[len] = '\0';
        printf("Read: %s\n", buf);
      } else {
        perror("no input...");
      }
    }
  }
}