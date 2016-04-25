// vim: ts=4:sts=4:et
#include <stdio.h>
#include <string>
#include <stack>
#include <cstdint>
#include <vector>
#include <map>

struct Exception {};

struct MyStack : public std::stack<uint16_t> {
  using B = std::stack<uint16_t>;

  void push(uint16_t v) {
    (v < (1<<12)) ? B::push(v) : throw Exception();
  }

  uint16_t pop() {
    if (size())
      throw Exception();
      
    auto v = top();
    B::pop();
    return v;
  }
};

int solution(std::string &S) {
  try {
    MyStack stack;
    for (auto c:S)
      stack.push(
        ('+' == c) ? (stack.pop() + stack.pop()) :
        ('*' == c) ? (stack.pop() * stack.pop()) : (c - '0')
      );

    return stack.pop();
  } catch (Exception& e) {
    return -1;
  }
}


int solution(const std::vector<int> &A) {
  unsigned cnt = 0;
  const int MAX = 1000000000;

  for (unsigned i = 0; i < A.size(); ++i) {
    int s = 0;
  for (unsigned j = i; j < A.size(); ++j) {
      s += A[j];
      if (0 == s) {
        if (++cnt > MAX) return -1;
      }
  }
  }

  return cnt;
}

int solution2(const std::vector<int> &A) {
  std::map<int, int> partSums;
  partSums[0] = 1;
  const int MAX = 1000000000;
  int sum = 0;
  unsigned cnt = 0;

  for (unsigned i = 0; i < A.size(); ++i) {
    sum += A[i];
    cnt += partSums[sum];
    partSums[sum]++;
    if (cnt >= MAX) return -1;
  }

  return cnt;
}


void test(const std::vector<int> &v) {
  printf("test ");
  char c = '{';
  for (const auto &x : v) {
    printf("%c%d", c, x);
    c = ',';
  }
  printf("} => %d | %d\n", solution(v), solution2(v));
}

int main(int argc, char* argv[]) {
  if (argc == 2) {
  std::string s(argv[1]);
  printf("{%s} => %d\n", s.c_str(), solution(s));
  return 0;
  }



  test({ 2,-2, 3, 0, 4,-7});

  test({ 0, 0, 0, 0, 0, 0, 0, 0});
  test({ 1, 2, 3, 1, 2, 3, 1, 2});
  test({ 1, 2, 3, 1, 2, 3, 1, 0});

  test({0,0,0,0,0,0,0,0,0,0});
  test({0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
       });
  return 0;
}

