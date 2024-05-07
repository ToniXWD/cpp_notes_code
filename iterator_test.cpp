#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main() {
  vector<int> vec1{1, 2, 3, 4, 5};
  for (auto it = vec1.begin(); it != vec1.end(); it++) {
    if (*it == 3) {
      it = vec1.erase(it);
    }
    cout << *it << "\t";
  }

  cout << '\n';

  vector<int> vec2{1, 2, 3, 4, 5};
  for (auto it2 = vec2.rbegin(); it2 != vec2.rend(); it2++) {
    if (*it2 == 3) {
      auto it = --(it2.base()); // 将反向迭代器转换为正向迭代器
      // 使用erase删除元素，并获取新的正向迭代器
      it = vec2.erase(it);
      // 从新的正向迭代器中获取新的反向迭代器
      it2 = std::reverse_iterator<decltype(it)>(it);
    }
    cout << *it2 << "\t";
  }

  cout << '\n';

  map<int, string> map1{{1, "1"}, {2, "2"}, {3, "3"}, {4, "4"}, {5, "5"}};

  for (auto it = map1.begin(); it != map1.end(); /* 现在不在这里递增it */) {
    if (it->first == 3) {
      // 删除当前元素，同时确保不会使迭代器失效
      // 递增迭代器，然后进行erase操作
      it = map1.erase(it);
    } else {
      cout << "(" << it->first << ", " << it->second << ")\t";
      ++it; // 只有在不删除元素时才递增迭代器
    }
  }
}
