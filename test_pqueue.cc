#include <gtest/gtest.h>
#include <functional>

#include "pqueue.h"

// Fill queue, pop until empty
TEST(PQueue, pop_til_empty) {
  PQueue<int> pq;

  pq.Push(30);
  pq.Push(20);
  pq.Push(10);

  EXPECT_EQ(pq.Top(), 10);
  EXPECT_EQ(pq.Size(), 3);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 20);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 30);
  pq.Pop();
  EXPECT_EQ(pq.Size(), 0);
}

// Min Queue
TEST(PQueue, less) {
  PQueue<int> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 2);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
}

// Bigger Min queue test
TEST(PQueue, more_less) {
  PQueue<int> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);
  pq.Push(50);
  pq.Push(55);
  pq.Push(60);
  pq.Push(4);

  EXPECT_EQ(pq.Top(), 2);
  EXPECT_EQ(pq.Size(), 8);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 23);
  EXPECT_EQ(pq.Size(), 6);
}
// Bigger max queue
TEST(PQueue, more_great) {
  PQueue<int, std::greater<int>> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);
  pq.Push(50);
  pq.Push(55);
  pq.Push(60);
  pq.Push(4);

  EXPECT_EQ(pq.Top(), 60);
  EXPECT_EQ(pq.Size(), 8);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 55);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 50);
  EXPECT_EQ(pq.Size(), 6);
}
// Max queue test
TEST(PQueue, great) {
  PQueue<int, std::greater<int>> pq;

  pq.Push(42);
  pq.Push(23);
  pq.Push(2);
  pq.Push(34);

  EXPECT_EQ(pq.Top(), 42);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), 34);
}

class MyClass {
 public:
  explicit MyClass(int n) : n_(n) {}
  bool operator < (const MyClass &mc) const { return n_ < mc.n_; }
  int n() { return n_; }
 private:
  int n_;
};

// Class object PQueue
TEST(PQueue, custom_class) {
  std::vector<MyClass> vec{MyClass(42), MyClass(23), MyClass(2), MyClass(34)};

  PQueue<MyClass> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top().n(), vec[2].n());
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top().n(), vec[1].n());
}

// Custom Comparator
template <class T> struct PtrComp {
  bool operator()(T *ptr, T *ptr_n){
    return *ptr < *ptr_n;
  }
};

// Pointer PQueue with custom comparator
TEST(PQueue, custom_class_pointer) {
  std::vector<MyClass*> vec{new MyClass(42), new MyClass(23),
                            new MyClass(2), new MyClass(34)};

  PQueue<MyClass*, PtrComp<MyClass>> pq;
  pq.Push(vec[0]);
  pq.Push(vec[1]);
  pq.Push(vec[2]);
  pq.Push(vec[3]);

  EXPECT_EQ(pq.Top(), vec[2]);
  EXPECT_EQ(pq.Size(), 4);
  pq.Pop();
  EXPECT_EQ(pq.Top(), vec[1]);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
