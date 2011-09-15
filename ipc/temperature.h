#ifndef BASEPID
#define BASEPID 70

/*
 * Data format for message passing
 */
struct {
  long priority;
  int temp;
  int pid;
  int stable;
} message;

#endif
