/* File:     pth_trap_sem.c
 * Purpose:  Compute a definite integral using the
 *           trapezoidal rule, Pthreads, and semaphores.
 *
 * Input:    a, b, n
 * Output:   Estimate of the integral from a to b of f(x)
 *           using the trapezoidal rule and n trapezoids.
 *
 * Access:   You should be able to run this program on any
 *           machine that supports Pthreads.  If you want
 *           to run it on a parallel machine, first log on
 *           to a node of the penguin cluster (see mpi_trap.c),
 *           grolsch, spaten, or stella.  Nodes of the penguin
 *           cluster have four cores.  Grolsch, spaten, and
 *           stella each have eight.
 * Compile:  Using gcc
 *           gcc -g -Wall -o pth_trap pth_trap.c -lpthread
 * Usage:    ./pth_trap <number of threads>
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
 * Note:  
 *    1.  f(x) is hardwired
 *    2.  Assumes the number of threads evenly divides the number of
 *        trapezoids.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

/* The global variables are shared among all the threads. */
int     thread_count;
double  a, b, h;
int     n, local_n;

sem_t   sem;   /* Use the semaphore as a mutex */
double  total;

void *Thread_work(void* rank);
double Trap(double local_a, double local_b, int local_n,
           double h);    /* Calculate local integral  */
double f(double x); /* function we're integrating */


/*--------------------------------------------------------------*/
int main(int argc, char** argv) {
    long        i;
    pthread_t*  thread_handles;  

    total = 0.0;
    if (argc != 2) {
       fprintf(stderr, "usage: %s <number of threads>\n", argv[0]);
       exit(0);
    }
    thread_count = strtol(argv[1], NULL, 10);
    printf("Enter a, b, n\n");
    scanf("%lf %lf %d", &a, &b, &n);
    h = (b-a)/n;
    local_n = n/thread_count;

    /* Allocate storage for thread handles.  Thread_count better */
    /* be positive . . .                                         */
    thread_handles = malloc (thread_count*sizeof(pthread_t));

    /* Initialize the semaphore */
    /* Second arg:  sem is shared among threads, not processes */
    /* Third arg:  initial value for sem = 1 = unlocked */
    sem_init(&sem, 0, 1);

    /* Start the threads.  Next to last argument is function in */
    /* which thread runs.  Final argument is thread's rank      */
    for (i = 0; i < thread_count; i++) {
         pthread_create(&thread_handles[i], NULL, Thread_work, 
               (void*) i);
    }

    /* Wait for threads to complete. */
    for (i = 0; i < thread_count; i++) {
        pthread_join(thread_handles[i], NULL);
    }

    printf("With n = %d trapezoids, our estimate\n",
         n);
    printf("of the integral from %f to %f = %f\n",
         a, b, total);

    sem_destroy(&sem);
    free(thread_handles);

    return 0;
} /*  main  */

/*--------------------------------------------------------------*/
void *Thread_work(void* rank) {
    double      local_a;   /* Left endpoint my thread   */
    double      local_b;   /* Right endpoint my thread  */
    double      my_int;    /* Integral over my interval */
    long        my_rank = (long) rank;

    /* Length of each process' interval of integration = */
    /* local_n*h.  So my interval starts at:             */
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;

    my_int = Trap(local_a, local_b, local_n, h);

    /* If sem > 0, sem-- and return */
    /* If sem == 0, wait until sem is nonzero */
    sem_wait(&sem);
    total += my_int;
    /* sem++ */
    sem_post(&sem);

    return NULL;

}  /* Thread_work */

/*--------------------------------------------------------------*/
double Trap(
          double  local_a   /* in */,
          double  local_b   /* in */,
          int     local_n   /* in */,
          double  h         /* in */) {

    double integral;   /* Store result in integral  */
    double x;
    int i;

    integral = (f(local_a) + f(local_b))/2.0;
    x = local_a;
    for (i = 1; i <= local_n-1; i++) {
        x = local_a + i*h;
        integral += f(x);
    }
    integral = integral*h;
    return integral;
} /*  Trap  */


/*--------------------------------------------------------------*/
double f(double x) {
    double return_val;

    return_val = x*x;
    return return_val;
} /* f */
