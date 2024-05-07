#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int signum) { printf("Received signal %d\n", signum); }

int main() {
  struct sigaction sa;

  // 设置信号处理函数
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  // 注册 SIGINT 的处理器
  if (sigaction(SIGINT, &sa, nullptr) == -1) {
    perror("sigaction");
    _exit(-1);
  }

  char buf[100];
  printf("Type:");
  if (fgets(buf, sizeof(buf), stdin) == nullptr) {
    perror("fgets failed");
  } else {
    printf("%s", buf);
  }

  return 0;
}