#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_XWD "SHM_XWD"
#define SHM_SIZE 4096

int main() {
  int shm_fd;
  char *shm_ptr;

  // 创建共享内存对象
  shm_fd = shm_open(SHM_XWD, O_RDWR | O_CREAT, 0666);

  if (shm_fd == -1) {
    perror("shm_open");
    return EXIT_FAILURE;
  }

  // 设置共享内存对象的大小
  if (ftruncate(shm_fd, SHM_SIZE) == -1) {
    perror("ftruncate");
    return EXIT_FAILURE;
  }

  // 将共享内存映射到指定地址
  shm_ptr = (char *)mmap(nullptr, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                         shm_fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("mmap");
    return EXIT_FAILURE;
  }

  sprintf(shm_ptr, "hello world");

  munmap(shm_ptr, SHM_SIZE);
  close(shm_fd);

  return 0;
}