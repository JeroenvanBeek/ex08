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
  
  pthread_t ThreadID_PT1;// producer thread 1 
  pthread_t ThreadID_PT2;// producer thread 2
  pthread_t ThreadID_PT3;// producer thread 3
  pthread_t ThreadID_CT;// consumer thread

  int arg_P1 = 1;
  int arg_P2 = 2;
  int arg_P3 = 3;
  int arg_P4 = 4;

  // Define SHR:
  memset(&act, '\0', sizeof(act));  // Fill act with NULLs by default
  act.sa_handler = ThreadStop;      // Set the custom SHR
  act.sa_flags = 0;                 // No flags, used with act.sa_handler
  sigemptyset(&act.sa_mask);        // No signal masking during SHR execution 
  
  // Install SHR:
  sigaction(SIGINT, &act, &oldact);  // This cannot be SIGKILL or SIGSTOP

  pthread_create(&ThreadID_PT1, NULL, ThreadFunction, (void*) &arg_P1);
  pthread_create(&ThreadID_PT2, NULL, ThreadFunction, (void*) &arg_P2);
  pthread_create(&ThreadID_PT3, NULL, ThreadFunction, (void*) &arg_P3);
  pthread_create(&ThreadID_CT, NULL, ThreadFunction, (void*) &arg_P4);

  createQueue(&queue, data);
  showQueue(&queue);

  while(killed)
  {

  }

  // Run data for the last time 
  pthread_join(ThreadID_PT1, NULL);
  pthread_join(ThreadID_PT2, NULL);
  pthread_join(ThreadID_PT3, NULL);
  pthread_join(ThreadID_CT, NULL);

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
    case 1: // writes data to shared queue every 2 seconds
    while(killed)
    {
        sleep(2);
        data.intVal = 1; // data is different, so it's possible to recognise the producer
        pushQueue(&queue, data);
    }
      break;         // End of thread function

    case 2: // writes data to shared queue every 3 seconds
    while(killed)
    {
        sleep(3);
        data.intVal = 5;// data is different, so it's possible to recognise the producer
        pushQueue(&queue, data);
    }
      break;         // End of thread function
    
    case 3: // writes data to shared queue every 4 seconds
    while(killed)
    {
        sleep(4);
        data.intVal = 9; //data is different, so it's possible to recognise the producer
        pushQueue(&queue, data); 
    }
      break;         // End of thread function

    case 4: // read data from shared queue every 15 seconds, print it and empties the queue
    while(killed)
    {
        sleep(15);
	    showQueue(&queue);
	    popQueue(&queue);
    }
      break;         // End of thread function
    }

pthread_exit(NULL);
}

