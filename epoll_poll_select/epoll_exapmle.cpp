#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

#define MAX_EVENTS 5
#define READ_SIZE 10
#define TIMEOUT 5000 // 设置超时时间为5秒（5000毫秒）

int main() {
  int epfd = epoll_create1(0);
  if (epfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLIN;
  ev.data.fd = STDIN_FILENO;

  if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
    perror("epoll_ctl: stdin");
    exit(EXIT_FAILURE);
  }

  while (1) {
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, TIMEOUT);
    if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int n = 0; n < nfds; ++n) {
      if (events[n].data.fd == STDIN_FILENO) {
        char buf[READ_SIZE + 1];
        ssize_t len = read(STDIN_FILENO, buf, READ_SIZE);
        if (len == -1) {
          perror("read");
          exit(EXIT_FAILURE);
        } else if (len) {
          buf[len] = '\0';
          printf("Read: %s\n", buf);
        }
      }
    }

    if (nfds == 0) {
      printf("Timeout.\n");
    }
  }

  close(epfd);
  return EXIT_SUCCESS;
}