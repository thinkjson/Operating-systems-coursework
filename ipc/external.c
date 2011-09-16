#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "temperature.h"

int main (int argc, char *argv[]) {
  int stable = 0;
  int status = 0;
  temperature_message message;
  temperature_message response;

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

  // Create message
  message.priority = 2;
  message.pid = offset;
  message.stable = 0;

  // Iterate as long as the system isn't stable
  while (stable == 0) {
    // Send updated temperature
    message.temp = temperature;
    status = msgsnd(outbox, &message, sizeof(message) - sizeof(long), 0);
    if (status < 0) {
        perror("Could not send message to central process");
    }

    // Receive response
    status = msgrcv(inbox, &response, sizeof(response) - sizeof(long), 0, 0);
    if (status >= 0) {
      temperature = response.temp;
      stable = response.stable;
    } else {
      perror("Could not receive message from central process");
    }

    // Make sure we can see what's going on
    fflush(stdout);

    // Make sure the message queue doesn't fill up faster than
    // the central process can remove them
    usleep(500000);
  }

  return 0;
}
