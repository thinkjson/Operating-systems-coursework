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
  int i;
  int stable_temps = 0;
  int total_temps = 0;
  temperature_message message;
  temperature_message response;

  // Prepare an array to hold the message queue ids
  int external_processes[MAXPROCS];
  memset(external_processes, 0, sizeof external_processes);
  int temperatures[MAXPROCS];
  memset(temperatures, -1, sizeof temperatures);

  // Grab parameters
  int temperature = atoi(argv[1]);
  int temperature_sum = 0;

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
      printf("%d => %d [",
        message.pid, message.temp);
      temperatures[message.pid] = message.temp;

      // Check to see if system has reached equillibrium
      stable_temps = 0;
      total_temps = 0;
      temperature_sum = temperature * 2;
      for (i = 0; i < MAXPROCS; i++) {
        if (temperatures[i] >= 0) {
          if (temperatures[i] == temperature) {
            stable_temps++;
          }

          temperature_sum += temperatures[i];
          total_temps++;
          printf("%d ", temperatures[i]);
        }
      }

      if (stable_temps == total_temps) {
        stable = 1;
      }
      temperature = temperature_sum / (2 + total_temps);
      printf("] <==> %d\n", temperature);

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
        response.stable = stable;
        response.temp = (message.temp * 3 + 2 * temperature) / 5;

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
  for (i = 0; i < MAXPROCS; i++) {
    if (external_processes[i] > 0) {
      status = msgsnd(external_processes[i], &response,
        sizeof(response) - sizeof(long), 0);
      if (status < 0) {
          perror("Could not tell external process to quit");
      }
      status = msgctl(external_processes[i], IPC_RMID, buf);
      if (status < 0) {
          perror("Could not remove mailbox");
      }
    }
  }

  return status == 0 ? 0 : 1;
}
