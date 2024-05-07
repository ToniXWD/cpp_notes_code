#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <new>

using namespace std;

class A {
  int a;
  char b;
  short c;

public:
  A() : a(0) {}
  A(int _a) : a(_a) {}
  const int &get_a() { return a; }
  void print() { cout << "A::print: a=" << a << endl; }
  static void *operator new(size_t size) {
    cout << "调用自定义的new A" << endl;
    A *alloc = (A *)malloc(sizeof(A));
    if (alloc == nullptr) {
      throw std::bad_alloc();
    }
    return alloc;
  }
  static void operator delete(void *addr) {
    cout << "调用自定义的free A" << endl;
    free(addr);
  }
};

class B : public A {
  long b;

public:
  B(int _a, int _b) : A(_a), b(_b) {}
  B() : A(), b(0) {}
  const long &get_b() { return b; }
  void print() {
    A::print();
    cout << "B::print: b=" << b << endl;
  }
  static void *operator new(size_t size) {
    A *alloc = (A *)malloc(sizeof(A));
    cout << "调用自定义的new B" << endl;
    if (alloc == nullptr) {
      throw std::bad_alloc();
    }
    return alloc;
  }
  static void operator delete(void *addr) {
    cout << "调用自定义的free B" << endl;
    free(addr);
  }
};

int main() {
  cout << sizeof(A) << endl;
  cout << sizeof(B) << endl;

  B b1(1, 2);
  A &a1 = b1;

  cout << sizeof(b1) << endl;
  cout << sizeof(a1) << endl;

  b1.print();
  a1.print();

  A *a2 = new A(10);
  a2->print();

  B *b2 = new B(10, 20);
  b2->print();

  delete a2;
  delete b2;
}