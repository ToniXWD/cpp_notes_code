#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <pid to attach>\n", argv[0]);
  }

  pid_t target_pid = atoi(argv[1]);
  int status;

  if (ptrace(PTRACE_ATTACH, target_pid, nullptr, nullptr) == -1) {
    perror("ptrace attach");
    return 1;
  }

  while (true) {
    waitpid(target_pid, &status, 0); // 等待target.c运行到sleep的位置
    if (WIFEXITED(status)) {
      break;
    }

    struct user_regs_struct regs; // #include <sys/user.h>
    ptrace(PTRACE_GETREGS, target_pid, nullptr, &regs);
    printf("RIP: %llx\n", regs.rip); // 打印指令指针的当前值

    ptrace(PTRACE_SINGLESTEP, target_pid, nullptr, nullptr);
  }

  ptrace(PTRACE_DETACH, target_pid, nullptr, nullptr);

  return 0;
}