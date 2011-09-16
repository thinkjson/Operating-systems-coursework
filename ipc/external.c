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
  temperature_message message;

  // Check for correct usage
  if (argc < 2) {
    printf("Usage: external [temperature] [pid offset]\n");
    exit(1);
  }

  // Grab parameters
  int temperature = atoi(argv[1]);
  int offset = atoi(argv[2]);

  printf("Initializing external process %d at temperature %d\n", offset, temperature);

  // Initialize inbox
  //int inbox = msgget(BASEPID + offset, 0600 | IPC_CREAT);
  //if (inbox < 0) {
//    printf("Could not initialize inbox");
    //exit(1);
  //}

  // Initialize outbox
  int outbox = msgget(BASEPID, 0600 | IPC_CREAT);
  if (outbox < 0) {
    printf("Could not initialize outbox");
    exit(1);
  }

  // Create message
  message.priority = 2;
  message.pid = offset;
  message.temp = temperature;
  message.stable = 0;

  // Iterate as long as the system isn't stable
  while (stable == 0) {
    printf("Sending message...\n", message.stable);
    status = msgsnd(outbox, &message, sizeof(message) - sizeof(long), 0);
    if (status < 0) {
        printf("Could not send message");
    }

    fflush(stdout);
    sleep(3);
  }

  // Free message queue
  struct msqid_ds msqid_ds, *buf;
  buf = & msqid_ds;
  status = msgctl(outbox, IPC_RMID, buf);

  return 0;
}
