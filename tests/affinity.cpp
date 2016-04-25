#include <stdint.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>

int main(int, const char*[]) {

  puts("hello world");

  cpu_set_t set;
  CPU_ZERO(&set);
  int res = sched_getaffinity(0, sizeof(set), &set);
  
  printf("ppid=%d pip=%d res=%d mask: %016lx\n", getppid(), getpid(), res,
         *(uint64_t*)&set);

  

  return 0;
}
