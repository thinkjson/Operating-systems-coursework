#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int size;

void *calc_row(void *threadarg) {
  typedef struct {
    int row;
    int matrix1[size][size];
    int matrix2[size][size];
    int result[size][size];
  } thread_data;
  thread_data *args = (thread_data *) threadarg;

  int cols, z;
  for (cols = 0; cols < size; cols++) {
    for (z = 0; z < size; z++) {
      args->result[args->row][z] += args->matrix1[args->row][cols] * args->matrix2[cols][z];
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  size = atoi(argv[1]);
  int rows, cols, z, status;
  void *thread_status;
  int matrix1[size][size], matrix2[size][size], result[size][size];
  printf("%*s%*d.0\t", 15, "pthreads", 15, size);

  // Set up pthreads
  pthread_t threads[size];
  typedef struct {
    int row;
    int matrix1[size][size];
    int matrix2[size][size];
    int result[size][size];
  } thread_data;
  thread_data thread_data_array[size];

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

  // Do the matrix multiplication
  for (rows = 0; rows < size; rows++) {
    thread_data_array[rows].row = rows;
    thread_data_array[rows].matrix1 = matrix1;
    thread_data_array[rows].matrix2 = matrix2;
    thread_data_array[rows].result = result;
    status = pthread_create(&threads[rows], NULL, calc_row,
        (void *) &thread_data_array[rows]);
    if (status) {
      perror("Could not create thread: ");
      exit(1);
    }

    status = pthread_join(threads[rows], &thread_status);
    if (status) {
      perror("Could not join thread: ");
      exit(1);
    }
  }

  pthread_exit(NULL);
  return 0;
}
