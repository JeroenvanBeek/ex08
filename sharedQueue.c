/******************************************************************************
 * File:         sharedQueue.c
 * Version:      1.0
 * Date:         2020-10-9
 * Author:       J. van Beek
 * Description:  OPS exercise 8: Queues
 ******************************************************************************/

#include "Queue.h"
#include <stdio.h>
#include <string.h>         // memset
#include <unistd.h>         // sleep() 
#include <signal.h>         // sigaction, sigemptyset, struct sigaction, SIGINT, kill()
#include <stdlib.h>
#include <pthread.h>

void *ThreadFunction(void *arg);
void ThreadStop(int sig);

int killed = 1;

data_t data = {1, "Hello queue"};
queue_t queue = {NULL};  // Note: element of queue = NULL

int main() 
{

  struct sigaction act, oldact;
  
  pthread_t ThreadID_A;
  pthread_t ThreadID_B;

  int arg_P1 = 1;
  int arg_P2 = 2;

  // Define SHR:
  memset(&act, '\0', sizeof(act));  // Fill act with NULLs by default
  act.sa_handler = ThreadStop;      // Set the custom SHR
  act.sa_flags = 0;                 // No flags, used with act.sa_handler
  sigemptyset(&act.sa_mask);        // No signal masking during SHR execution 
  
  // Install SHR:
  sigaction(SIGINT, &act, &oldact);  // This cannot be SIGKILL or SIGSTOP

  pthread_create(&ThreadID_A, NULL, ThreadFunction, (void*) &arg_P1);
  pthread_create(&ThreadID_B, NULL, ThreadFunction, (void*) &arg_P2);

  createQueue(&queue, data);
  showQueue(&queue);

  while(killed)
  {

  }
  
  pthread_join(ThreadID_A, NULL);
  pthread_join(ThreadID_B, NULL);

  deleteQueue(&queue);

return 0;
}

// SHR using sa_handler:
void ThreadStop(int sig)
{
    printf("\n Stop thread's\n");
    killed = 0;
}

void *ThreadFunction(void *arg)
{

    int *s_arg= (int*) arg;

    switch(*s_arg)
    {
    case 1:
    while(killed)
    {
        data.intVal = 1;
        pushQueue(&queue, data);
        sleep(2);
    }
      break;         // End of thread function
    case 2:
    while(killed)
    {
        data.intVal = 5;
        pushQueue(&queue, data);
        sleep(3);
	    showQueue(&queue);
	  
	    popQueue(&queue);
    }
      break;         // End of thread function
    }
pthread_exit(NULL);
}
/*  printf("\nList the contents of the current queue:\n");
  showQueue(&queue);
  
  printf("\nCreate a new queue:\n");
  createQueue(&queue, data);
  showQueue(&queue);
  data.intVal++;
  
  
  
  printf("\nAdd new data to the queue:\n");
  pushQueue(&queue, data);
  showQueue(&queue);
  data.intVal++;
  
  printf("\nAdd new data to the queue:\n");
  pushQueue(&queue, data);
  showQueue(&queue);
  data.intVal++;
  
  printf("\nAdd new data to the queue:\n");
  pushQueue(&queue, data);
  showQueue(&queue);
  
  printf("\nRemove data from the queue:\n");
  popQueue(&queue);
  showQueue(&queue);
  
  
  
  printf("\nFront iValue/first node: %d\n", frontQueue(&queue)->intVal);
  printf("Back  iValue/last node:  %d\n", backQueue(&queue)->intVal);
  
  
  printf("\nDelete the current queue:\n");
  deleteQueue(&queue);
  showQueue(&queue);
 
  
  printf("\nCreate a new queue:\n");
  createQueue(&queue, data);
  showQueue(&queue);
  deleteQueue(&queue);
  data.intVal++;
  return 0;
}*/ 
