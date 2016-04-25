/*
        10        20        30        40        50        60        70        80        90       100
12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234
*/
#include <cstdio>
#include <iostream>
#include <cstdint>

struct MyClass {
	template<typename T> explicit MyClass(const T &x) {
		std::cout << "this is x " << x << std::endl;
	}
};

void print(const MyClass &x) {
	std::cout << "sizeof MyClass: " << sizeof(x) << std::endl;
}

struct Hdr {
	uint32_t length;
	uint16_t type;
	constexpr Hdr(uint32_t l, uint16_t t) : length(l), type(t) { }
};

struct Msg1 : public Hdr {
	const static uint16_t Type = 1;
	char body[111];
	template<size_t N> Msg1(const char (&s)[N]) : Hdr(sizeof(*this), Type) {
		for (size_t i = 0; i < N && i < 111; ++i) body[i] = s[i];
	}
	void print() const { std::cout << "msg1[" << body << "]" << std::endl; }
};

struct Msg2 : public Hdr {
	const static uint16_t Type = 2;
	char body[222];
	template<size_t N> Msg2(const char (&s)[N]) : Hdr(sizeof(*this), Type) {
		for (size_t i = 0; i < N && i < 111; ++i) body[i] = s[i];
	}
	void print() const { std::cout << "msg2[" << body << "]" << std::endl; }
};

struct Msg3 : public Hdr {
	const static uint16_t Type = 3;
	int x;
	Msg3(int x_) : Hdr(sizeof(*this), Type), x(x_) { };
	void print() const { std::cout << "msg3{" << x << "}" << std::endl; }
};

template<typename H, typename Msg>
struct A_ {
	static void handleHdr(H &, const Hdr &) { throw "wtf";
//		std::cout << "unknown type: " << h.type << std::endl;
	}

};

template<typename H, typename Msg, typename B = A_<H, Msg> >
struct A : public B {
	using B::handleHdr;

	static void handleHdr(H &h, const Hdr &hdr) {
		(hdr.type == Msg::Type) ? h.handle(static_cast<const Msg&>(hdr))
                                  : B::handleHdr(h, hdr);
	}
};

struct MyClass2 {
 	struct H {
 		MyClass2 &c;
 		H(MyClass2 &c_) : c(c_) { }
		void handleHdr(const Hdr &hdr) { A<H, Msg1, A<H, Msg2> >::handleHdr(*this, hdr); }
 		template<typename T> void handle(const T &m) { c.handle(m); }
 	};

	MyClass2() { std::cout << "MyClass2::ctor()" << std::endl; }

 	__attribute__((noinline)) void handle(const Msg1 &m) {
		std::cout << "got Msg1" << std::endl;
		m.print();
	}
 	__attribute__((noinline)) void handle(const Msg2 &m) {
		std::cout << "got Msg2" << std::endl;
		m.print();
	}

	using Handler = A<MyClass2, Msg1, A<MyClass2, Msg2> >;
 
 	__attribute__((noinline)) void handle(const Hdr &hdr) {
		Handler::handleHdr(*this, hdr);
 	}
};

namespace O {
struct Hdr { int type; };
struct MsgA : public Hdr { enum _ { Type = 1 }; int a,b,c; MsgA() : Hdr{Type} {} };
struct MsgB : public Hdr { enum _ { Type = 2 }; int a,b,c; MsgB() : Hdr{Type} {} };

void recvA(const MsgA &) { std::cout << "recvA" << std::endl; }
void recvB(const MsgB &) { std::cout << "recvB" << std::endl; }

void recv(const Hdr &h) {
  (h.type == MsgA::Type) ? recvA(static_cast<const MsgA&>(h)) :
  (h.type == MsgB::Type) ? recvB(static_cast<const MsgB&>(h)) : throw "bad type";
}
}

#include <functional>

struct Executor {
	using Fn = std::function<void(int)>;
	Fn m_fn;

	template<typename F> __attribute__((noinline)) void schedule(const F &f) {
		std::cout << "Executor::schedule. sizeof Fn: " << sizeof(Fn)
				  << " f: " << sizeof(f) << std::endl;
		f(123);
		m_fn = f;
	}

	__attribute__((noinline)) void launch() const {
		std::cout << "Executor::launch. " << std::endl;
		m_fn(100500);
	}
};

__attribute__((noinline)) void doSomething(int x) { std::cout << "x: " << x << std::endl; }


struct So {
	constexpr static int Jo = 66;
};

#include <functional>



int main(int, const char*[]) {
	MyClass x(3.2);
	std::cout << "hello world" << std::endl;
	print(x);
//	print(53);

	Msg1 m1a("my msg1a"), m1b("my msg1b");
	Msg2 m2a("my msg2a"), m2b("my msg2b");
	Msg3 m3(44);

	try {

	MyClass2 c;

	c.handle(m1a);
	c.handle(m1b);
	c.handle(m2a);
	c.handle(m2b);
	c.handle(m3);
	} catch(const char* s) {
		std::cout << "something went wrong! err: " << s << std::endl;
	}

	try {
	O::recv(O::MsgA());
	O::recv(O::MsgB());
	O::recv(O::Hdr{4});
	} catch (const char* s) { std::cout << "err:" << s << std::endl; }

	Executor e;
	{
	    char dabadam[] = "dabadam dabadam dabadam dabadam";
		int x = 7;
	asm("nop;nop;nop;");
		e.schedule([&x, &dabadam](int a) {
			std::cout << "x: " << x << ", dabadam: " << dabadam << ", a: " << a << std::endl;
		});
	asm("nop;nop;nop;");
		x = 8;
	    e.launch();
	}
	e.launch();
	enum _ { A, B, C };

	doSomething(A);
	doSomething(+So::Jo);
	doSomething(3);

	asm("nop;nop;nop;");
	auto hash = std::hash<std::string>{}("stringa"); 
	asm("nop;nop;nop;");

	std::cout << "hash of 777: " << hash << std::endl;

	return 0;
}
