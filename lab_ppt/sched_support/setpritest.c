#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int values[] = {2, 1, 0, 3, -1};
  int expected[] = {0, 0, -1, -1, -1};
  int failed = 0;

  for (int i = 0; i < 5; i++) {
    int result = setpri(values[i]);

    printf("setpri(%d) = %d, expected %d\n",
           values[i], result, expected[i]);

    if (result != expected[i])
      failed = 1;
  }

  if (failed) {
    printf("setpritest: FAIL\n");
    exit(1);
  }

  printf("setpritest: return values OK\n");
  exit(0);
}
