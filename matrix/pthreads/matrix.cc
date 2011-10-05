#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#define MAX_SIZE 1000

int size;
int matrix1[MAX_SIZE][MAX_SIZE], matrix2[MAX_SIZE][MAX_SIZE], result[MAX_SIZE][MAX_SIZE];
std::queue<int> thread_queue;

void *calc_row(void *args) {
  int row, cols, z;
  while (thread_queue.size() > 0) {
    row = thread_queue.front();
    thread_queue.pop();

    for (cols = 0; cols < size; cols++) {
      for (z = 0; z < size; z++) {
        result[row][z] += matrix1[row][cols] * matrix2[cols][z];
      }
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  size = atoi(argv[1]);
  int rows, cols, status;
  void *thread_status;
  printf("%*s%*d.0\t", 15, "pthreads", 15, size);

  // Check matrix size
  if (size > MAX_SIZE) {
    printf("Matrix too large\n");
    exit(1);
  }

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
  //printf("Generated matrices\n");

  // Get CPU information
  long nprocs = sysconf(_SC_NPROCESSORS_ONLN) * 2;
  if (nprocs <= 0) {
    printf("No processors are available to this process.");
    exit(1);
  }
  //printf("Using %ld threads to multiply matrices of size %d\n", nprocs, size);

  // Do the matrix multiplication
  for (rows = 0; rows < size; rows++) {
    thread_queue.push(rows);
  }
  //printf("Thread queue populated\n");

  // Set up pthreads
  pthread_t threads[nprocs];
  int n;
  for (n = 0; n < nprocs; n++) {
    // Create thread
    status = pthread_create(&threads[n], NULL, calc_row, NULL);
    if (status) {
      perror("Could not create thread: ");
      exit(1);
    } //else {
      //printf("Created thread %d\n", n);
    //}

    // Join thread
    status = pthread_join(threads[n], &thread_status);
    if (status) {
      perror("Could not join thread: ");
      exit(1);
    }
  }

  pthread_exit(NULL);
  return 0;
}
