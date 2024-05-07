#include <iostream>
#include <type_traits>

long add(long a, long b) { return a + b; }

int main() {
  typedef std::result_of<decltype(add) *(long, long)>::type result_type;

  std::cout << "Result type of add function: " << typeid(result_type).name()
            << std::endl;

  return 0;
}
