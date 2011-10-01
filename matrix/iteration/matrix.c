#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  int size = atoi(argv[1]), rows, cols;
  int matrix1[size][size], matrix2[size][size], result[size][size];
  printf("%*s%*d.0\t", 15, "Iterative", 15, size);

  // Generate matrices
  srand(time(NULL));

  for (rows = 0; rows < size; rows++) {
    for (cols = 0; cols < size; cols++) {
      matrix1[rows][cols] = rand();
      matrix2[rows][cols] = rand();
      sleep(0);
    }
  }

  // Do the matrix multiplication
  for (rows = 0; rows < size; rows++) {
    for (cols = 0; cols < size; cols++) {
      result[rows][cols] = 0;
    }
  }

  return 0;
}
