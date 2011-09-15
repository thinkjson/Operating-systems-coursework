#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "temperature.h"

int main (int argc, char *argv[]) {
  int stable = 0;
  int status = 0;

  // Check for correct usage
  if (argc < 2) {
    printf("Usage: external [temperature] [pid offset]\n");
    exit(1);
  }

  // Grab parameters
  int temperature = atoi(argv[1]);
  int offset = atoi(argv[2]);

  // Initialize inbox
  int inbox = msgget(BASEPID + offset, 0600 | IPC_CREAT);
  if (inbox < 0) {
    printf("Could not initialize inbox");
    exit(1);
  }

  // Initialize outbox
  int outbox = msgget(BASEPID, 0600 | IPC_CREAT);
  if (outbox < 0) {
    printf("Could not initialize outbox");
    exit(1);
  }

  // Iterate as long as the system isn't stable
  message.pid = offset;
  message.temp = temperature;
  while (stable == 0) {
    status = msgsnd(outbox, &message, sizeof(message) - sizeof(long), 0);
    sleep(1);
  }

  // Free message queue
  struct msqid_ds msqid_ds, *buf;
  buf = & msqid_ds;
  status = msgctl(inbox, IPC_RMID, buf);

  return 0;
}
