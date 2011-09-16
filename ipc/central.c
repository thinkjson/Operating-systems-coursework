#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "temperature.h"

int main (int argc, char *argv[]) {
  int stable = 0;
  int status = 0;
  temperature_message message;
  temperature_message response;

  // Prepare an array to hold the message queue ids
  int external_processes[MAXPROCS];
  memset(external_processes, 0, sizeof external_processes);

  // Grab parameters
  int temperature = atoi(argv[1]);

  // Initialize inbox
  int inbox = msgget(BASEPID, 0600 | IPC_CREAT);
  if (inbox < 0) {
      printf("Could not initialize inbox\n");
      exit(1);
  }

  // Iterate as long as the system isn't stable
  while (stable == 0) {
    // Receive message
    status = msgrcv(inbox, &message, sizeof(message) - sizeof(long), 0, 0);
    if (status >= 0) {
      printf("External process %d reports temperature %d\n",
        message.pid, message.temp);

      // Initialize mailbox if it isn't already
      if (external_processes[message.pid] == 0) {
        external_processes[message.pid] = msgget(BASEPID + message.pid, 0600 | IPC_CREAT);
        if (external_processes[message.pid] < 0) {
            printf("Could not initialize outbox for process %d", message.pid);
            external_processes[message.pid] = 0;
        }
      }

      // Send response
      if (external_processes[message.pid] > 0) {
        response.priority = 2;
        response.pid = 0;
        response.temp = message.temp;
        response.stable = 0;

        status = msgsnd(external_processes[message.pid], &response,
          sizeof(response) - sizeof(long), 0);
        if (status < 0) {
            perror("Could not send message");
        }
      }

    } else {
      perror("Error occurred while receiving message");
    }

    // Flush buffer so we know what's going on
    fflush(stdout);
  }

  // Free inbox
  struct msqid_ds msqid_ds, *buf;
  buf = & msqid_ds;
  status = msgctl(inbox, IPC_RMID, buf);
  if (status < 0) {
      printf("Could not remove mailbox");
  }

  // Free outboxes
  int i;
  for (i = 0; i < MAXPROCS; i++) {
    if (external_processes[i] > 0) {
      status = msgctl(external_processes[i], IPC_RMID, buf);
      if (status < 0) {
          printf("Could not remove mailbox");
      }
    }
  }

  return status == 0 ? 0 : 1;
}
