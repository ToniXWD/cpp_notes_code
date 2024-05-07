#include <compare>
#include <iostream>

using namespace std;

class Point {
public:
  int x, y;

  std::strong_ordering operator<=>(const Point &other) const {
    if (auto cmp = x <=> other.x; cmp != 0)
      return cmp;         // 先比较x
    return y <=> other.y; // 如果x相等，再比较y
  }
};

void main() {
  Point p1{1, 2};
  Point p2{1, 2};
  Point p3{2, 3};

  // 使用<=>生成的比较
  auto res = p1 <=> p2;
  std::cout << (p1 <= = > p2) << std::endl; // 输出 1 （true）
  std::cout << (p1 != p3) << std::endl;     // 输出 1 （true）
  std::cout << (p1 < p3) << std::endl;      // 输出 1 （true）
  std::cout << (p3 > p1) << std::endl;      // 输出 1 （true）
}