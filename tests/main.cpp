// vim: ts=4:sts=4:et
//
#include <stdint.h>
#include <new>

#include <stdio.h>
#include <rns/Strings.hpp>

struct BufRef {
  struct Data_ {
    uint16_t m_size;
    char* data(uint16_t i) { return ((char*)&this[1]) + i; }
    const char* data(uint16_t i) const { return ((const char*)&this[1]) + i; }
    uint16_t size() const { return m_size; }
  protected:
    Data_() : m_size(0) {}
  };

  template <size_t N>
  struct Data : public Data_{
    Data() : Data_() {}

    char m_data[N];
  };

  enum NoInit_ {NoInit};
  BufRef(NoInit_) {}

  void set(BufRef::Data_& d, const char* s) {
    m_ref = d.m_size;
    uint16_t len;
    for (len = 0; s[len]; ++len) d.data(d.m_size)[len] = s[len];
    m_len = len;
    d.m_size += len;
  }

  uint16_t length() const { return m_len; }
  const char *data(const BufRef::Data_& d) const { return d.data(m_ref); }

  uint16_t m_ref;
  uint16_t m_len;
};

struct Pkt {

  template <typename A, typename B>
  Pkt(const A& a, const B& b) : m_a(BufRef::NoInit), m_b(BufRef::NoInit) {
    m_a.set(m_data, a);
    m_b.set(m_data, b);
  }

  BufRef m_a;
  BufRef m_b;

  BufRef::Data<200> m_data;
};


void recv(const Pkt& pkt) {
  printf("got a packet max size %lu pkt.data.size %u, a = {%.*s} b = {%.*s}\n",
         sizeof(Pkt), pkt.m_data.size(), pkt.m_a.length(), pkt.m_a.data(pkt.m_data),
         pkt.m_b.length(), pkt.m_b.data(pkt.m_data));
}


template<size_t offs, size_t cnt, size_t N> void handle(const char (&s)[N]) {
    printf("total:%lu offs:%lu cnt:%lu {%.*s}\n", N, offs, cnt, static_cast<int>(cnt), &s[offs]);  
}

// --
#include <type_traits>
#include <utility>

template<class F> struct wrapper {
    static_assert(std::is_empty<F>(), "Lambdas must be empty");
    template<class... Ts>
    decltype(auto) operator()(Ts&&... xs) const {
        return reinterpret_cast<const F&>(*this)(std::forward<Ts>(xs)...);
    }
};

template<class F> constexpr wrapper<F> make_function(F*) { return {}; }

struct wrapper_factor {
    template<class F>
    constexpr wrapper<F> operator += (F*) { return {}; }
};

struct addr_add {
    template<class T> friend typename std::remove_reference<T>::type *
    operator+(addr_add, T &&t) { return &t; }
};
#define STATIC_LAMBDA wrapper_factor() += true ? nullptr : addr_add() + []


uint64_t stou(const char *d) {
    uint64_t r = (*d++) - '0';
    if (r > 9) return r;
    do {
        uint8_t c = (*d++) - '0';
        if (c > 9) return r;
        r = r * 10 + c;
    } while (1);
}

void parsefix(const char *data) {
    printf("Parse {%s}: %lu\n", data, stou(data));
}


struct alignas(128) MyAlignedC {
    char x[3];
    int y;
};

#include <cstddef>

// const auto add_one = [](auto x) { return x + 1; };
const constexpr auto add_one = STATIC_LAMBDA(auto x) { return x + 1; };

int main(int argc, const char* argv[]) {
    printf("got %d args:\n", argc);
	for (int i = 0; i < argc; ++i)
		printf("%d: {%s}\n", i, argv[i]);


    char data[1024];
    Pkt *pkt = new (data) Pkt("sup", "gonna be long");

	puts("receiving packet");
	recv(*pkt);

    puts("test strings");
    String<64> s("this is a string");
    puts("string:");
    s.print();
    puts("done");

    handle<3,7>("This is a very long string");

    int x = add_one(5);
    printf("x = add_one(5) = %d, sizeof(add_one) = %lu\n", x, sizeof(add_one));

    if (argc > 1) parsefix(argv[1]);


    {
        MyAlignedC a;
        char somedata[256];
        MyAlignedC *b = new MyAlignedC();
        auto test = reinterpret_cast<size_t>(b) % alignof(MyAlignedC);
        MyAlignedC *c = (MyAlignedC*)&somedata[3];


        printf("max align allowed: %lu\n", alignof(std::max_align_t));
        printf("sizeof(MyAlignedC): %lu, a:%p b:%p => %lu c_:%p => c (c_+3):%p\n",
               sizeof(MyAlignedC), (void*)&a, (void*)b, test, (void*)&somedata[0], (void*)c);

        delete b;
    }

    return 0;
}

