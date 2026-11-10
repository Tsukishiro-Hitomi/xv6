#include "kernel/types.h"
#include "user/user.h"

struct result {
  int pid;
  int priority;
  uint start;
  uint finish;
};

static void
fail(void)
{
  printf("schedtest: setup or communication failed\n");
  exit(1);
}

int
main(int argc, char *argv[])
{
  int priority[2];
  int gate[2];
  int report[2];
  char token;
  struct result results[2];

  if (argc != 3) {
    printf("usage: schedtest priority1 priority2\n");
    exit(1);
  }

  for (int i = 0; i < 2; i++) {
    priority[i] = atoi(argv[i + 1]);
    if (priority[i] != 1 && priority[i] != 2) {
      printf("priority must be 1 or 2\n");
      exit(1);
    }
  }

  if (pipe(gate) < 0 || pipe(report) < 0)
    fail();

  // Keep the controller responsive during setup.
  if (setpri(2) != 0)
    fail();

  for (int i = 0; i < 2; i++) {
    int pid = fork();
    if (pid < 0)
      fail();

    if (pid == 0) {
      close(gate[1]);
      close(report[0]);

      if (setpri(priority[i]) != 0)
        fail();

      // Tell the parent we are ready, then wait for release.
      if (write(report[1], "R", 1) != 1)
        fail();
      if (read(gate[0], &token, 1) != 1)
        fail();
      close(gate[0]);

      struct result r;
      r.pid = getpid();
      r.priority = priority[i];
      r.start = uptime();

      volatile uint work = 0;
      for (uint n = 0; n < 100000000U; n++)
        work = work + 1;

      r.finish = uptime();

      if (write(report[1], &r, sizeof(r)) != sizeof(r))
        fail();
      close(report[1]);
      exit(0);
    }
  }

  close(gate[0]);
  close(report[1]);

  // Wait until both children have set their priorities.
  for (int i = 0; i < 2; i++) {
    if (read(report[0], &token, 1) != 1)
      fail();
  }

  uint t0 = uptime();
  if (write(gate[1], "GG", 2) != 2)
    fail();
  close(gate[1]);

  // Collect results without printing during the computation.
  for (int i = 0; i < 2; i++) {
    if (read(report[0], &results[i], sizeof(results[i]))
        != sizeof(results[i]))
      fail();
  }
  close(report[0]);

  wait(0);
  wait(0);

  printf("Times are ticks since release.\n");
  for (int i = 0; i < 2; i++) {
    printf("pid=%d priority=%d start=%d finish=%d\n",
           results[i].pid, results[i].priority,
           (int)(results[i].start - t0),
           (int)(results[i].finish - t0));
  }

  exit(0);
}
