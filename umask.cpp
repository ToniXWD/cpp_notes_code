#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd;
  mode_t old_mask;  //#include <sys/types.h>

  // 设置新的 umask 值为 022 (屏蔽掉组和其他用户的写权限)
  old_mask = umask(022);

  fd = open("test01.txt", O_WRONLY | O_CREAT, 0666);
  //   尝试设置权限为 0666（可读可写）。由于 umask 的设置，实际的文件权限将是
  //   0644（0666 & ~022）。

  if (fd < 0) {
    perror("open");
    _exit(-1);
  }

  const char* buf = "hello world";
  int size = strlen(buf);
  write(fd, buf, size + 1);

  close(fd);

  return 0;
}