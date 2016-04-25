#include <iostream>
#include <cstring>

// helpers for printing
template <typename T> struct Op {
  static void print(const T &v) { std::cout << v; }
};
template <typename T> void print(const T &v) { Op<T>::print(v); }

// double-linked list with only methods we need
template <typename T> class List {
  List(const List&) = delete;
  List &operator=(const List&) = delete;

  struct Node {
    template<typename... Args>
    Node(Node *n, Node *p, Args... params)
      : next(n), prev(p), data(params...) { }

    Node  *next;
    Node  *prev;
    T     data;
  };

public:
  class CIterator {
    friend class List<T>;
    CIterator(const Node *v) : m_current(v) { }

  public:
    CIterator(const CIterator&) = default;
    CIterator &operator=(const CIterator&) = default;

    operator bool() const { return nullptr != m_current; }
    const T &operator*() const { return m_current->data; }

    CIterator &iterate() {
      if (nullptr != m_current) m_current = m_current->next;
      return *this;
    }

  private:
    const Node *m_current;
  };

  List() = default;
  ~List() {
    while (!empty()) pop_back();
  }

  template<typename... Args> void emplace_back(Args... params) {
    Node *n = new Node(nullptr, m_back, params...);
    m_back = (nullptr == m_front) ? (m_front = n) : (m_back->next = n);
  }

  void pop_back() {
    Node *n = m_back;
    if (nullptr == n) return;
    if (nullptr == n->prev) {
      m_front = m_back = nullptr;
    } else {
      m_back = n->prev;
      m_back->next = nullptr;
    }
    delete n;
  }

  bool many() const { return m_front != m_back; }
  bool empty() const { return nullptr == m_front; }
  T &back() { return m_back->data; }
  CIterator cbegin() const { return CIterator(m_front); }

private:
  Node  *m_front{nullptr};
  Node  *m_back{nullptr};
};

template <typename T> class MemoryManager {
  MemoryManager(const MemoryManager &) = delete;
  MemoryManager &operator=(const MemoryManager &) = delete;

  struct Node {
    T       *ptr;
    size_t  length;
    Node(T *p, size_t l) : ptr(p), length(l) { }
  };

  void init() {
    for (size_t i = 0; i < m_length; ++i) {
      if (m_data[i] != 0) continue;
      auto j = i + 1;
      for (; j < m_length && m_data[j] == 0; ++j) ;
      m_list.emplace_back(&m_data[i], j - i);
      i = j;
    }
  }

  void print_free() const {
    std::cout << "Free block " << (m_list.many() ? "lengths" : "length");
    auto i = m_list.cbegin();
    if (i) {
      std::cout << ": " << (*i).length;
      while (i.iterate()) std::cout << ", " << (*i).length;
    } else {
      std::cout << ": 0";
    }
  }

  void print_occupied() const {
    std::cout << "Occupied block contents: ";
    T *from = m_data;
    auto i = m_list.cbegin();
    auto *to = i ? (*i).ptr : &m_data[m_length];
    bool comma = false;
    do {
      if (comma) std::cout << ", ";
      comma = from < to;

      for (; from < to; ++from) ::print(*from);
      if (!i) break;
      
      from += (*i).length;
      to = i.iterate() ? (*i).ptr : &m_data[m_length];
    } while (from < to);
  }

  void defragment_() {
    if (m_list.empty()) return;

    // merge free blocks in backward order until get only one left
    while (m_list.many()) {
      auto *free_ptr = m_list.back().ptr;
      auto free_len = m_list.back().length;
      m_list.pop_back();
      auto &back = m_list.back();
      std::memmove(back.ptr + back.length + free_len, back.ptr + back.length,
                   sizeof(T) * (free_ptr - back.ptr - back.length));
      back.length += free_len;
    }

    if (m_list.back().ptr == m_data) return;

    // move free block to the beginning of array
    std::memmove(m_data + m_list.back().length, m_data,
                 sizeof(T) * (m_list.back().ptr - m_data));
    m_list.back().ptr = m_data;
  }

public:
  MemoryManager(T *data, size_t length)
    : m_data(data), m_length(length) { init(); }

  void print() const {
    print_free();
    std::cout << " | ";
    print_occupied();
    std::cout << std::endl;
  }

  MemoryManager &defragment() {
    defragment_();
    return *this;
  }

private:
  T           *m_data;
  size_t      m_length;
  List<Node>  m_list;
};

// print integers
template<> void Op<int>::print(const int &v) { std::cout << static_cast<char>(v); }

int main()
{
  int array[] = {
     0,   0,   0,               // free block of 3
    'C', 'O', 'N', 'T', 'I',    // occupied block of 5
     0,   0,   0,   0,   0,     // free block of 5
    'G', 'U', 'O', 'U',         // occupied block of 4
     0,   0,                    // free block of 2
    'S', '!' };                 // occupied block of 2

  MemoryManager<int> mm(array, 21);
  mm.print();
  // output: Free block lengths: 3, 5, 2 | Occupied block contents: CONTI, GUOU, S!
  mm.defragment().print();
  // output: Free block length: 10 | Occupied block contents: CONTIGUOUS!

  return 0;
}
