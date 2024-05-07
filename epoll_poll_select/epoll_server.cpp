#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main() {
  int server_fd, new_socket, epoll_fd;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};

  // 创建 socket 文件描述符
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // 设置 socket 选项
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // 绑定 socket 到指定地址
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // 开始监听
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // 创建 epoll 实例
  epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLIN;
  ev.data.fd = server_fd;

  // 将服务器 socket 注册到 epoll 实例
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
    perror("epoll_ctl: server_fd");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // 等待事件发生
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int n = 0; n < nfds; ++n) {
      if (events[n].data.fd == server_fd) {
        // 如果有新的连接，接受它并添加到 epoll 实例
        new_socket = accept(server_fd, (struct sockaddr *)&address,
                            (socklen_t *)&addrlen);
        if (new_socket < 0) {
          perror("accept");
          exit(EXIT_FAILURE);
        }

        ev.events = EPOLLIN;
        ev.data.fd = new_socket;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &ev) == -1) {
          perror("epoll_ctl: new_socket");
          exit(EXIT_FAILURE);
        }
      } else {
        // 如果是已连接的客户端发生了读取事件，读取数据
        ssize_t count = read(events[n].data.fd, buffer, BUFFER_SIZE);
        if (count == -1) {
          perror("read");
          exit(EXIT_FAILURE);
        } else if (count == 0) {
          // 如果客户端关闭连接，关闭 socket 并从 epoll 中移除
          close(events[n].data.fd);
        } else {
          // 打印接收到的数据
          printf("Received: %s\n", buffer);
          write(events[n].data.fd, buffer, BUFFER_SIZE);
          memset(buffer, 0, sizeof(buffer));
        }
      }
    }
  }

  // 关闭服务器 socket
  close(server_fd);
  // 关闭 epoll 文件描述符
  close(epoll_fd);

  return 0;
}