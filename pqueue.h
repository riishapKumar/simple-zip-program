#ifndef PQUEUE_H_
#define PQUEUE_H_

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>
#include <utility>

template <typename T, typename C = std::less<T>>
class PQueue {
 public:
  // Constructor
  PQueue() = default;
  // Return number of items in priority queue
  size_t Size();
  // Return top of priority queue
  T& Top();
  // Remove top of priority queue
  void Pop();
  // Insert item and sort priority queue
  void Push(T item);

 private:
  std::vector<T> items;
  size_t cur_size = 0;
  C cmp;

  // Helper methods for indices
  size_t Root() {
    return 0;
  }
  size_t Parent(size_t n) {
    return (n - 1) / 2;
  }
  size_t LeftChild(size_t n) {
    return 2 * n + 1;
  }
  size_t RightChild(size_t n) {
    return 2 * n + 2;
  }

  // Helper methods for node testing
  bool HasParent(size_t n) {
    return n != Root();
  }
  bool IsNode(size_t n) {
    return n < cur_size;
  }

  // Helper methods for restructuring
  void PercolateUp(size_t n) {
    while (HasParent(n) && CompareNodes(n, Parent(n))) {
      std::swap(items[Parent(n)], items[n]);
      n = Parent(n);
    }
  }
  void PercolateDown(size_t n) {
    // While node has at least one child (if one, necessarily on the left)
    while (IsNode(LeftChild(n))) {
    // Consider left child by default
      size_t child = LeftChild(n);
      // If right child exists and smaller/greater than left child,
      // then consider right child
      if (IsNode(RightChild(n)) && CompareNodes(RightChild(n), LeftChild(n)))
        child = RightChild(n);
      // Exchange smallest/greater child with node
      if (CompareNodes(child, n)) {
        std::swap(items[child], items[n]);
      } else {
        break;
      }
    // Do it again, one level down
    n = child;
    }
  }

  // Node comparison
  bool CompareNodes(size_t i, size_t j) {
    if (cmp(items[i], items[j]))
      return true;
    else
      return false;
  }
};

// To be completed below

// Public ADT definitions
template<typename T, typename C>
size_t PQueue<T, C>::Size() {
  return cur_size;
}
template<typename T, typename C>
T &PQueue<T, C>::Top() {
  if (Size() == 0)
    throw std::underflow_error("Empty priority queue!");
  return items[Root()];
}
template<typename T, typename C>
void PQueue<T, C>::Pop() {
  if (Size() == 0)
    throw std::underflow_error("Empty priority queue!");
  cur_size--;
  items[Root()] = std::move(items[cur_size]);
  PercolateDown(Root());
  items.pop_back();
}
template<typename T, typename C>
void PQueue<T, C>::Push(T item) {
  // Insert at the end
  items.push_back(item);
  items[cur_size] = std::move(item);
  // Percolate up
  PercolateUp(cur_size);
  cur_size++;
}

#endif  // PQUEUE_H_
