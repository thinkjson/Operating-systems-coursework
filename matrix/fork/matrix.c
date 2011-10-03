#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define BASEPID 9000
#define MAX_SIZE

int matrix1[10][10], matrix2[10][10], result[10][10];

typedef struct {
  long priority;
  int
  int *row[];
  int *col[];
  int *result[];
  int finished;
} matrix;

int main(int argc, char *argv[]) {
  int size = atoi(argv[1]), rows, cols, z, PID, proc, parent = 1, modulo,
      inbox, outbox, mailbox;
  matrix message;

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

  // Initialize inbox
  inbox = msgget(BASEPID, 0600 | IPC_CREAT);
  if (inbox < 0) {
    printf("Could not initialize inbox");
    exit(1);
  }

  // Fire up processes
  long nprocs = sysconf(_SC_NPROCESSORS_ONLN) * 2;
  if (nprocs <= 0) {
    printf("No processors are available to this process.");
    exit(1);
  }
  int processes[nprocs], mailboxes[nprocs];
  for (proc = 0; proc < nprocs; proc++) {
    // Initialize inbox
    mailbox = msgget(BASEPID + 1 + proc, 0600 | IPC_CREAT);
    if (mailbox < 0) {
      printf("Could not initialize inbox");
      exit(1);
    }

    PID = fork();
    if (PID == 0) { // Child process
      parent = 0;
      modulo = proc;
      outbox = inbox;
      inbox = mailbox;
      break;
    } else { // Parent process
      processes[proc] = PID;
      mailboxes[proc] = mailbox;
    }
  }

  // Do the matrix multiplication
  if (parent == 1) {
    // Populate queues
    for (rows = 0; rows < size; rows++) {
      message.row = matrix1[rows][cols];
      message.col = matrix2[cols][z];
      message.result = result[rows][z];
      status = msgsnd(mailboxes[rows % nprocs], &message, sizeof(message) - sizeof(long), 0);
    }

    // Notify processes to end
    for (proc = 0; proc < nprocs; proc++) {
      message.finished = 1;
      status = msgsnd(mailboxes[proc], &message, sizeof(message) - sizeof(long), 0);
    }

    // Wait on children before exiting
    for (proc = 0; proc < nprocs; proc++) {
      waitpid(processes[proc]);
    }
    printf("Parent finished");

    // Print finished matrix
    for (rows = 0; rows < size; rows++) {
      for (cols = 0; cols < size; cols++) {
        printf("%d ", result[rows][cols]);
      }
      printf("\n");
    }

    //printf("%*s%*d.0\t", 15, "fork()", 15, size);
  } else {
    // Block on queue input, return resultant row
    while (message.finished != 1) {
      status = msgrcv(inbox, &message, sizeof(response) - sizeof(long), 0, 0);
      for (cols = 0; cols < size; cols++) {
        for (z = 0; z < size; z++) {
          message.result += message.row * message.col;
        }
      }
    }
  }

  return 0;
}
