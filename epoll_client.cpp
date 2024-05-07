#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE] = {0};
  char message[BUFFER_SIZE];
  int sock = 0;

  // 创建 socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket creation error");
    return -1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);

  // 将 IP 地址从文本转换为二进制形式
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    perror("Invalid address/ Address not supported");
    return -1;
  }

  // 连接到服务器
  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Connection Failed");
    return -1;
  }

  while (true) {
    memset(buffer, 0, sizeof(buffer));   // 清空缓冲区
    memset(message, 0, sizeof(message)); // 清空缓冲区

    scanf("%s", message);
    // 发送消息
    send(sock, message, strlen(message), 0);

    // 接收服务器响应
    ssize_t valread = read(sock, buffer, BUFFER_SIZE);
    if (valread > 0) {
      printf("Received from server: %s\n", buffer);
    } else if (valread == 0) {
      printf("Server closed the connection\n");
    } else {
      perror("recv");
    }
  }

  // 关闭 socket
  close(sock);

  return 0;
}