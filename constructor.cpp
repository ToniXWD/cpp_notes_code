#include <iostream>

class A {
public:
  int value;

  // 默认构造函数
  A() : value(0) {
    // 在这里可以打印消息以证明构造函数被调用
    std::cout << "Constructor called" << std::endl;
  }

  // 拷贝构造函数
  A(const A &other) : value(other.value) {
    // 在这里可以打印消息以证明拷贝构造函数被调用
    std::cout << "Copy constructor called" << std::endl;
  }
};

int main() {
  A x;     // 调用默认构造函数
  A y = x; // 调用拷贝构造函数
  return 0;
}