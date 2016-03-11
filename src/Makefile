all: trap_mutex trap_semaphore trap_busywaiting 


trap_mutex : trap.c
	gcc -Wall -DD_MUTEX -o trap_mutex trap.c -lpthread

trap_semaphore : mutex.c
	gcc -Wall -DD_SEMAPHORE -o trap_semaphore trap.c -lpthread

trap_busywaiting : semaphore.c
	gcc -Wall -DD_BUSYWAITING -o trap_busywaiting trap.c -lpthread

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.out
	rm -f trap_*
