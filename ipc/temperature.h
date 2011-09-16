#ifndef BASEPID
#define BASEPID 8000
#define MAXPROCS 32

/*
 * Data format for message passing
 */
typedef struct {
  long priority;
  int temp;
  int pid;
  int stable;
} temperature_message;

#endif
