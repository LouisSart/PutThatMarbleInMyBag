#include <array>
#include <cassert>
#include <deque>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <vector>

template <typename T, typename... Ts>
void print(const T &truc, const Ts &...reste) {
  if constexpr (sizeof...(Ts) == 0) {
    std::cout << truc << std::endl;
  } else {
    std::cout << truc << " ";
    print(reste...);
  }
}

template <unsigned N> struct SumMatrix : std::array<int, N * N> {
  SumMatrix() {
    this->fill(0);
    for (unsigned k = 0; k < N; ++k) {
      (*this)[k * (N + 1)] = N;
    }
  }

  auto apply(int i, int j, int ii, int jj) {
    assert((*this)[i * N + j] > 0);
    assert((*this)[ii * N + jj] > 0);
    (*this)[i * N + j] -= 1;
    (*this)[ii * N + jj] -= 1;
    (*this)[i * N + jj] += 1;
    (*this)[ii * N + j] += 1;
  }

  bool is_possible_move(int i, int j, int ii, int jj) const {
    if (j == jj)
      return false;
    if ((*this)[i * N + j] == 0 || (*this)[ii * N + jj] == 0)
      return false;
    return true;
  }

  auto make_children() {
    SumMatrix m;
    std::vector<SumMatrix> ret;
    for (int i = 0; i < (int)(N - 1); ++i) {
      for (int j = 0; j < (int)N; ++j) {
        for (int ii = i + 1; ii < (int)N; ++ii) {
          for (int jj = 0; jj < (int)N; ++jj) {
            if (is_possible_move(i, j, ii, jj)) {
              m = *this;
              m.apply(i, j, ii, jj);
              ret.push_back(m);
            }
          }
        }
      }
    }
    return ret;
  }

  long unsigned hash() const {
    long unsigned ret = 0, p = 1;
    for (unsigned r = 0; r < N - 1; ++r) {
      for (unsigned c = 0; c < N - 1; ++c) {
        ret += (long unsigned)(p * (*this)[r * N + c]);
        p *= (long unsigned)(N + 1);
      }
    }
    return ret;
  }
  void show() const {
    for (unsigned r = 0; r < N; ++r) {
      std::cout << "[";
      for (unsigned c = 0; c < N; ++c) {
        std::cout << std::setw(2) << (*this)[r * N + c] << " ";
      }
      std::cout << "\b] ";
      std::cout << std::endl;
    };
  }
};

template <unsigned N> std::unordered_set<long unsigned> count_matrices() {
  auto root = SumMatrix<N>();
  std::deque<SumMatrix<N>> queue{root};
  std::unordered_set<long unsigned> bag{root.hash()};

  while (queue.size() > 0) {
    auto M = queue.back();
    for (auto child : M.make_children()) {
      long unsigned h = child.hash();
      if (!bag.contains(h)) {
        queue.push_front(child);
      }
      bag.insert(h);
    }
    queue.pop_back();
  }
  return bag;
}

int main() {
  print(count_matrices<1>().size());
  print(count_matrices<2>().size());
  print(count_matrices<3>().size());
  print(count_matrices<4>().size());
  // print(count_matrices<5>().size()); // takes nearly 20 minutes on my PC
}