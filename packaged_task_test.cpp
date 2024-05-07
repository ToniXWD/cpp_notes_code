#include <functional>
#include <future>
#include <iostream>
#include <thread>

int add(int a, int b) { return a + b; }

int main() {
  std::packaged_task<int(int, int)> task(
      add);  // 创建一个packaged_task，包装add函数
  std::future<int> result =
      task.get_future();  // 获取与packaged_task关联的future

  std::thread taskThread(std::move(task), 10, 20);  // 在新线程中执行任务

  int res = result.get();  // 获取任务执行的结果
  std::cout << "Result: " << res << std::endl;

  taskThread.join();  // 等待任务线程执行完毕

  return 0;
}
