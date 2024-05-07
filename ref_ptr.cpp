#include <iostream>

template <typename T> class RefPtr {
public:
  // 构造函数
  explicit RefPtr(T *ptr = nullptr) : ptr_(ptr), count_(new size_t(1)) {
    if (ptr == nullptr) {
      // 如果传入的是空指针，则将引用计数设置为0
      *count_ = 0;
    }
  }

  // 复制构造函数
  RefPtr(const RefPtr &other) : ptr_(other.ptr_), count_(other.count_) {
    if (ptr_) {
      // 如果指针非空，则增加引用计数
      ++(*count_);
    }
  }

  // 赋值运算符
  RefPtr &operator=(const RefPtr &other) {
    if (this != &other) { // 防止自赋值
      // 减少当前对象的引用计数，如果变为0，则删除
      release();

      // 复制数据
      ptr_ = other.ptr_;
      count_ = other.count_;
      if (ptr_) {
        // 如果指针非空，则增加引用计数
        ++(*count_);
      }
    }
    return *this;
  }

  // 析构函数
  ~RefPtr() { release(); }

  // 重载解引用操作符
  T &operator*() const { return *ptr_; }

  // 重载箭头操作符
  T *operator->() const { return ptr_; }

  // 获取原始指针
  T *get() const { return ptr_; }

private:
  T *ptr_;        // 指向被引用计数管理的对象
  size_t *count_; // 引用计数

  void release() {
    if (ptr_ && --(*count_) == 0) {
      delete ptr_;
      delete count_;
      ptr_ = nullptr;
      count_ = nullptr;
    }
  }
};

// 测试代码
class Test {
public:
  Test() { std::cout << "Test created\n"; }
  ~Test() { std::cout << "Test destroyed\n"; }
  void greet() { std::cout << "Hello, world!\n"; }
};

int main() {
  RefPtr<Test> ptr1(new Test()); // 创建一个Test对象
  {
    RefPtr<Test> ptr2 = ptr1; // 使用复制构造函数
    ptr2->greet();
  } // ptr2超出作用域，但Test对象不会被删除，因为ptr1还在引用它
  ptr1->greet();
} // ptr1超出作用域，Test对象将被删除