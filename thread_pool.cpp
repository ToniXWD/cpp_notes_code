#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
public:
  ThreadPool(size_t);
  template <class F, class... Args>
  auto enqueue(F &&f, Args &&...args)
      -> std::future<typename std::result_of<F(Args...)>::type>;
  ~ThreadPool();

private:
  // 需要保持追踪线程，所以使用一个线程向量
  std::vector<std::thread> workers;
  // 任务队列
  std::queue<std::function<void()>> tasks;

  // 同步
  std::mutex queue_mutex;
  std::condition_variable condition;
  bool stop;
};

// 线程池的构造函数启动一些数量的工作线程
ThreadPool::ThreadPool(size_t threads) : stop(false) {
  for (size_t i = 0; i < threads; ++i)
    workers.emplace_back([this] {
      for (;;) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(this->queue_mutex);
          this->condition.wait(
              lock, [this] { return this->stop || !this->tasks.empty(); });
          if (this->stop && this->tasks.empty())
            return;
          task = std::move(this->tasks.front());
          this->tasks.pop();
        }

        task();
      }
    });
}

// 添加新的工作到线程池中
template <class F, class... Args>
auto ThreadPool::enqueue(F &&f, Args &&...args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex);

    // 不允许在停止 ThreadPool 后入队新的任务
    if (stop)
      throw std::runtime_error("enqueue on stopped ThreadPool");

    tasks.emplace([task]() { (*task)(); });
  }
  condition.notify_one();
  return res;
}

// 销毁所有线程
ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex);
    stop = true;
  }
  condition.notify_all();
  for (std::thread &worker : workers)
    worker.join();
}

int main() {
  ThreadPool pool(4);

  // 启动并发任务
  auto result = pool.enqueue([](int answer) { return answer; }, 42);

  // 获取异步操作的结果
  std::cout << result.get() << std::endl;

  return 0;
}