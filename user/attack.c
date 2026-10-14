#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)
  char *end = sbrk(PGSIZE * 32);
  char str[] = "my very very very secret pw is:   ";
  for (int i = 0; i < 32; i++) {
    char *start = end + PGSIZE * i;
    if (memcmp(start + 8, str + 8, 24) == 0) {

      /**information for debugging
      result: page found at 16*/

      // printf("page found at %d.\n", i);
      
      start += 32;
      write(2, start, 8);
      exit(0);
    }
  }
  exit(1);
}
