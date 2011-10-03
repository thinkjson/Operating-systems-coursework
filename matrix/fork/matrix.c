#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int size = atoi(argv[1]), rows, cols, z, PID, proc, parent;
  int matrix1[size][size], matrix2[size][size], result[size][size];
  printf("%*s%*d.0\t", 15, "Iterative", 15, size);

  // Generate matrices
  srand(time(NULL));
  for (rows = 0; rows < size; rows++) {
    for (cols = 0; cols < size; cols++) {
      matrix1[rows][cols] = rand();
      matrix2[rows][cols] = rand();
      result[rows][cols] = 0;
      sleep(0);
    }
  }

  // Fire up processes
  long nprocs = sysconf(_SC_NPROCESSORS_ONLN) * 2;
  if (nprocs <= 0) {
    printf("No processors are available to this process.");
    exit(1);
  }
  int processes[nprocs];
  PID = fork();
  for (proc = 0; proc < nprocs; proc++) {
    if (PID == 0) {
      parent = 0;
      break;
    } else {
      parent = 1;
      processes[proc] = PID;
      PID = fork();
    }
  }

  // Do the matrix multiplication
  if (parent == 1) {
    printf("In parent\n");
    // Populate queues
    for (rows = 0; rows < size; rows++) {
      for (cols = 0; cols < size; cols++) {
        for (z = 0; z < size; z++) {
          result[rows][z] += matrix1[rows][cols] * matrix2[cols][z];
        }
      }
      printf("Calculated row %d", rows);
    }

    // Wait on children before exiting
    for (proc = 0; proc < nprocs; proc++) {
      waitpid(processes[proc]);
    }
  } else {
    // Loop over queue and wait for termination signal
    printf("In child\n");
  }

  return 0;
}
