/*
 * Purpose:  Compute a integral with the trapezoidal rule using POSIX Threads.
 *
 * Input:    One value for inferior limit -> a
 	     One value for the superior limit -> b
	     The number of sub-intervals -> n
 *
 * Output:   Estimate the integral from a to b of f(x) using Pthreads for the trapezoidal rule and n sub-intervals
 *
 *
 */
#include <stdio.h>      /* For I/0 needed: scanf and printf **/
#include <stdlib.h>
#include <pthread.h>    

#ifdef D_SEMAPHORE
#include <semaphore.h>  /* Semaphores need to be included because is not part of Pthreads */
#endif

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
/* Global variables can introduce subtle and confusing bugs! */
/* Limit use of global variables to situations in which they re really needed.*/
/* The global variables are shared among all the threads. */
int thread_count;
double a;
double b;
double h;
int n;
int n_thread;
double all_approx;


#ifdef D_SEMAPHORE
sem_t sem;   /* Declaration of semaphore */
#endif


#ifdef D_MUTEX
pthread_mutex_t mutex;	/* The "Special" type for mutexes - Slide 35 */
#endif

#ifdef D_BUSYWAITING
long flag = 0;
#endif

/*** Global Functions ***/
void Usage (char* prog_name);
void *job_thread(void* rank); 								/* Thread function */
double trapezoidal_rule(double a_thread, double b_thread, int n_thread, double h);	/* Calculate integral for thread */
double f(double x); 


/*----------------- MAIN ----------------------------------*/
int main(int argc, char** argv) {
    long thread_id;
    pthread_t* thread_handles;  
    all_approx = 0.0f;	/* For being used with semaphore */
    //int get_out = 0;	/* Flag USER-FRIENDLY Interface I/O while */
    
    /* Get number of threads from command line */
    char* prog_name = argv[0];
    if (argc < 5) Usage(prog_name);
    thread_count = strtol(argv[1], NULL, 10);
    if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

    /******************** I/O Script VERSION ******************************/
    
    /** Interval [a,b] **/
    a = strtol(argv[2], NULL, 10);
    b = strtol(argv[3], NULL, 10);

    /** Number of Sub-intervals/rectangles **/
    n = strtol(argv[4], NULL, 10); 
   
    /*********************************************************************/


   
    /***************** USER-FRIENDLY VERSION *******************************
    while(!get_out && 1){
	    puts("Insert the values of [a,b] interval");
	    printf("Inferior limit (a): \n");
	    scanf("%lf", &a);
	    printf("Superior limit (b): \n");
	    scanf("%lf", &b);
	    if (a > b) printf("a should be smaller than b!"); 
	    else get_out = 1;
    }
    get_out = 0;
    while(!get_out && 1){
    	printf("Insert the number of Sub-intervals: \n");
	scanf("%d", &n);
	if (n <= 0) puts("Not a valid interval\n");
	else get_out = 1;
    }
    ***********************************************************************/
    h = (b-a)/n;

    /*** Pre-condition! We should divide the number of Sub-intervals EQUALLY to the number of threads we're executing the program! Otherwise, the result will not be correct ***/
    n_thread = n/thread_count;

    thread_handles = malloc (thread_count*sizeof(pthread_t));
    
    
#ifdef D_MUTEX
    /******* Initiate MUTEX ******/
    pthread_mutex_init(&mutex, NULL);
#endif

    
#ifdef D_SEMAPHORE
    /******* Initiate SEMAPHORE ******/
    sem_init(&sem, 0, 1); /** 0 - semaphore shared by all threads, 1 - Semaphore initialized as unlocked **/
#endif

    /* Create Pthreads*/
    for (thread_id = 0; thread_id < thread_count; thread_id++) {
         pthread_create(&thread_handles[thread_id], NULL, job_thread, (void*) thread_id);
    }

    /* Waiting threads to join */
    for (thread_id = 0; thread_id < thread_count; thread_id++) {
        pthread_join(thread_handles[thread_id], NULL);
    }

    /* All threads finished their work. Main print the result */
    
    
    printf("%s,%d,%f,%f,%d,%f\n",prog_name,thread_count,a,b,n,all_approx);


    /*********  PRINT USER-FRIENDLY ******
    printf("From [%f,%f] the integral estimate with trapezoide-rule with %d sub-intervals is: %f\n", a, b, n, all_approx);
   **************************************/


#ifdef D_SEMAPHORE
    /*** Destroy the semaphore ***/
    sem_destroy(&sem);
#endif


#ifdef D_MUTEX
    /**** Destroy the mutex ***/
    pthread_mutex_destroy(&mutex);
#endif

    free(thread_handles);

return 0;
} 

/*--------------Each thread sould run this job --------------------------------*/
void *job_thread(void* rank) {
    long my_rank = (long) rank;
    double a_thread;   
    double b_thread;   
    double my_integral;    

    a_thread = a + my_rank*n_thread*h;
    b_thread = a_thread + n_thread*h;

    my_integral = trapezoidal_rule(a_thread, b_thread, n_thread, h);


#ifdef D_SEMAPHORE
					/* If sem is zero thread waits until is != zero */
					/* If sem if not zero, thread enter critical region */
    sem_wait(&sem);			/* Entering the critical region and LOCK the semaphore sem */
     	all_approx += my_integral;	/* Update all_approx safely */
    sem_post(&sem);			/* Exit the critical region and RELEASE the semaphore by incrementing sem */
#endif



#ifdef D_MUTEX
    pthread_mutex_lock(&mutex);		/* Entering the critical region and LOCK the mutex */
    	all_approx += my_integral;	/* Update all_approx safely */
    pthread_mutex_unlock(&mutex);	/* Exit critical region and UNLOCK de mutex for other thread can use */
#endif



#ifdef D_BUSYWAITING
    while(flag != my_rank){
    	all_approx += my_integral;
	flag = (flag+1) % thread_count;
    }
#endif

return NULL;
}


/*------------ Algorithm for one aproximation interval -----------------------*/
double trapezoidal_rule(double a_thread, double b_thread, int n_thread, double  h) {

    double approx;   
    double x_i;
    int i;

    approx = (f(a_thread)+f(b_thread))/2.0;
    for (i = 1; i <= n_thread-1; i++) {
        x_i = a_thread + i*h;
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
	fprintf(stderr, "Make sure you're passing %s NTHREADS a b n as arguments", prog_name);	
	fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
	fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
exit(0);
}
