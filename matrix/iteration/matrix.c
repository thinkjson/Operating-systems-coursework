#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int size = atoi(argv[1]), rows, cols;
  printf("%*s%*d.0\t", 15, "Iterative", 15, size);

  // Generate matrices

  for (rows = 0; rows < size; rows++) {
    for (cols = 0; cols < size; cols++) {
      // Calculate matrix multiplication
    }
  }

  return 0;
}
