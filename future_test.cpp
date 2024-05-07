#include <future>
#include <iostream>
#include <thread>

int find_the_answer_to_ltuae() {
  // 假装我们在计算"生命、宇宙以及任何事情的终极答案"
  return 42;
}

void do_work(std::promise<int> &p) {
  // 做一些工作...
  p.set_value(111); // 设置 promise 的值，这将通知 future
}

int test_1() {
  // 启动一个异步任务
  std::future<int> the_answer = std::async(find_the_answer_to_ltuae);

  // 做一些其他的事情...

  // 在需要结果的时候等待异步任务完成并获取结果
  std::cout << "The answer is " << the_answer.get() << std::endl;

  return 0;
}

int test_2() {
  // 创建一个 promise 对象
  std::promise<int> p;

  // 获取与 promise 共享状态相关联的 future 对象
  std::future<int> f = p.get_future();

  // 在一个线程中完成一些工作，并设置 promise 的值
  std::thread t1(do_work, std::ref(p));

  // 等待线程设置 future 的值，并获取它
  std::cout << "The answer is " << f.get() << std::endl;

  // 确保线程在退出前完成
  t1.join();

  return 0;
}

int main() {
  test_1();
  test_2();
}