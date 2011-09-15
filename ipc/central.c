#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include "temperature.h"

int main (int argc, char *argv[]) {
  int stable = 0;
  int status = 0;

  // Initialize inbox
  int inbox = msgget(BASEPID, 0600 | IPC_CREAT);
  if (inbox < 0) {
      printf("Could not initialize inbox");
      exit(1);
  }

  // Iterate as long as the system isn't stable
  while (stable == 0) {
    status = msgrcv(inbox, &message, sizeof(message) - sizeof(long), 0, 2);
    printf("External process %d reports temperature %d",
        message.pid, message.temp);
    stable = 1;
  }

  // Free message queue
  struct msqid_ds msqid_ds, *buf;
  buf = & msqid_ds;
  status = msgctl(inbox, IPC_RMID, buf);

  return status == 0 ? 0 : 1;
}
