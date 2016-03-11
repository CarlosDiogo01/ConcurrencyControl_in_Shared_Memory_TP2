/* 
 * Purpose:
 *	Illustrate basic use of pthreads:  create some threads, each of which prints a message.
 *	In this program i'll measure how much time is needed for creating and terminate a single thread 
 *	For this prupose i'll use timer.h that Professor António Pina gave for his students. 
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  gcc -g -Wall -o pth_hello pth_hello.c -lpthread
 * Usage:    ./pth_hello <thread_count>
 *
 * IPP:   Section 4.2 (p. 153 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include "timer.h"
#include <math.h>	/* For using pow to convert measured times in seconds, to miliseconds */

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
/* Global variables can introduce subtle and confusing bugs! */
/* Limit use of global variables to situations in which they re really needed.*/
int thread_count;  

void Usage(char* prog_name);

void *Hello(void* rank);  /* Thread function */

/*---------------------- MAIN ---------------------------------------------*/
int main(int argc, char* argv[]) {

   long       thread;			/* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   double start, finish, elapsed;	/* For measuring time with Professor Antonio Pina timer.h */ 
	
   /* Get number of threads from command line */
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

   thread_handles = malloc (thread_count*sizeof(pthread_t)); 

   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], 
                     NULL, 
                     Hello, 
                     (void*) thread);  

   //printf("Output:\n");
   //printf("Hello from the main thread\n");

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 

   GET_TIME(finish);
   elapsed = finish - start;
  
   //printf("NThreads,Average_Time_Per_Thread(ms),Time(ms)\n");
   printf("%d,%f,%f\n", thread_count, (float)((elapsed/thread_count)*pow(10,3)), elapsed*pow(10,3));
   
   free(thread_handles);
   return 0;
}  /* main */

/*-------------------------------------------------------------------*/
/* Hello */
void *Hello(void* rank) {
   //long my_rank = (long) rank;  /* Use long in case of 64-bit system */ 

   //printf("Hello from thread %ld of %d\n", my_rank, thread_count);

   return NULL;
}  

/*-------------------------------------------------------------------*/
/* Usage */
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}  
