#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  // 文件描述符集合
  fd_set readfds;
  // 超时时间结构体
  struct timeval tv;
  // 用于select返回后，检测结果的返回值
  int retval;

  // 设置超时时间为5秒
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  // 循环等待输入
  while (1) {
    // 清空文件描述符集合
    FD_ZERO(&readfds);
    // 将标准输入（stdin）加入文件描述符集合
    FD_SET(STDIN_FILENO, &readfds);

    // 等待输入，参数1为最大文件描述符加1
    retval = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

    if (retval == -1) {
      // select出错
      perror("select()");
      exit(EXIT_FAILURE);
    } else if (retval) {
      // 数据可读
      printf("Data is available now.\n");
      // 文件描述符准备就绪，可以进行读取操作
      if (FD_ISSET(STDIN_FILENO, &readfds)) {
        char buf[1024];
        ssize_t len = read(STDIN_FILENO, buf, sizeof(buf) - 1);
        if (len > 0) {
          buf[len] = '\0';
          printf("Read: %s\n", buf);
        }
      }
    } else {
      // 超时，没有文件描述符准备就绪
      printf("No data within five seconds.\n");
    }

    // 重置超时时间，因为select调用可能会更新tv结构
    tv.tv_sec = 5;
    tv.tv_usec = 0;
  }

  return EXIT_SUCCESS;
}