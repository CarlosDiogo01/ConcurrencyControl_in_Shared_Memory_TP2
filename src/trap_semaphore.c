/*
 * Purpose:  Compute a integral with the trapezoidal rule using Pthreads, and semaphores.
 *
 * Input:    a, b, n
 * Output:   Estimate of the integral from a to b of f(x) using the trapezoidal rule and n trapezoids.
 *
 * Algorithm:
 *    1.  Each thread calculates "its" interval of
 *        integration.
 *    2.  Each thread estimates the integral of f(x)
 *        over its interval using the trapezoidal rule.
 *    3.  Each thread adds in its calculation into the
 *        the global total
 *    4.  main prints result
 *
 */
#include <stdio.h>      /* For I/0 needed: scanf and printf **/
#include <stdlib.h>
#include <pthread.h>    
#include <semaphore.h>  /* Semaphores need to be included because is not part of Pthreads */

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
/* Global variables can introduce subtle and confusing bugs! */
/* Limit use of global variables to situations in which they re really needed.*/
/* The global variables are shared among all the threads. */
int thread_count;
double  a, b, h;
int     n, n_thread;
double total;

sem_t sem;   /* Declaration of semaphore */

void Usage (char* prog_name);
void *job_thread(void* rank); 								/* Thread function */
double trapezoidal_rule(double a_thread, double b_thread, int n_thread, double h);	/* Calculate integral for thread */
double f(double x); 


/*----------------- MAIN ----------------------------------*/
int main(int argc, char** argv) {
    long thread;
    pthread_t* thread_handles;  
    total = 0.0;	/* For being used with semaphore */
    
    /* Get number of threads from command line */
    if (argc != 2) Usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

    printf("Insert:\n Inferior limit (a), Superior limit (b), from [a,b], and n_Sub-intervals\n");
    scanf("%lf %lf %d", &a, &b, &n);
    
    thread_handles = malloc (thread_count*sizeof(pthread_t));
    h = (b-a)/n;
    n_thread = n/thread_count;

    /******* Initialize the semaphore ******/
    sem_init(&sem, 0, 1); /** 0 - semaphore shared by all threads, 1 - Semaphore initialized as unlocked **/

    /* Create Pthreads*/
    for (thread = 0; thread < thread_count; thread++) {
         pthread_create(&thread_handles[thread], NULL, job_thread, (void*) thread);
    }

    /* Waiting threads to join */
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    /* All threads finished their work. Main print the result */
    printf("From [%f,%f] the estimate with %d sub-intervals is: %19.15e\n", a, b, n, total);

    
    /******* Destroy the semaphore ******/
    sem_destroy(&sem);

    free(thread_handles);
    return 0;
} 

/*--------------Each thread sould run this job --------------------------------*/
void *job_thread(void* rank) {
    long my_rank = (long) rank;
    double a_thread;   
    double b_thread;   
    double my_int;    

    a_thread = a + my_rank*n_thread*h;
    b_thread = a_thread + n_thread*h;

    my_int = trapezoidal_rule(a_thread, b_thread, n_thread, h);

    sem_wait(&sem);
    total += my_int;
    sem_post(&sem);

    return NULL;

}

/*------------ Algorithm for one aproximation interval -----------------------*/
double trapezoidal_rule(double a_thread, double b_thread, int n, double  h) {

    double approx;   
    double x_i;
    int i;

    approx = (f(a)+f(b))/2.0;
    for (i = 1; i <= n-1; i++) {
        x_i = a + i*h;
        approx += f(x_i);
    }
    approx = approx*h;

return approx;
}
/*-------------- f(x) --------------*/
double f(double x) {
    double return_val;

    return_val = x*x;
    return return_val;
}
/*-------------------------------------------------------------------*/
/* Usage */
void Usage(char* prog_name) {
	fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
	fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
	exit(0);
} 
