#include <stdint.h>
#include <stdio.h>
#include <atomic>
#include <future>

// vim: !g++53 -std=c++11 -lpthread -O3 ./threads.cpp -o ./threads.bin && ./threads.bin

class spin_lock {
	spin_lock(const spin_lock&) = delete;
	spin_lock& operator=(const spin_lock&) = delete;
public:
	spin_lock() = default;

	void lock() {
		while (m_lock.test_and_set(std::memory_order_acquire)) asm("pause");
	}
	bool try_lock() { return !m_lock.test_and_set(std::memory_order_acquire); }
	void unlock() { m_lock.clear(std::memory_order_release); }

private:
	std::atomic_flag m_lock{ATOMIC_FLAG_INIT};
};

void f(spin_lock& l) {
  printf("f\n");
  l.lock();
  printf("f done\n");
}

void g(spin_lock& l) {
  bool res = l.try_lock();
  printf("g: %s\n", res ? "locked" : "not locked");
}

int main(int, char*[]) {
  printf("threads\n");

#define NO_LOCK_CHECK(T) do { \
    T x; \
    printf(#T "::is_lock_free: %c\n", x.is_lock_free() ? 'Y' : 'N'); \
  } while (0)

  NO_LOCK_CHECK(std::atomic_int);
  NO_LOCK_CHECK(std::atomic_char);
  NO_LOCK_CHECK(std::atomic<uint64_t>);
  NO_LOCK_CHECK(std::atomic_long);

  spin_lock lock;

  {
    std::async(std::launch::async, [&lock]{ f(lock); });
    std::async(std::launch::async, [&lock]{ g(lock); });  // does not run until f() completes
  }

  printf("main ending\n");
  lock.lock();
  printf("main done\n");
  lock.unlock();

  return 0;
}
