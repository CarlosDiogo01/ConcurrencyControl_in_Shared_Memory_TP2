all: pth_hello trap_mutex trap_semaphore trap_busywaiting 

pth_hello : pth_hello.c
	gcc -Wall -g -o pth_hello pth_hello.c -lpthread

trap_mutex : trap.c
	gcc -Wall -g -DD_MUTEX -o trap_mutex trap.c -lpthread

trap_semaphore : trap.c
	gcc -Wall -g -DD_SEMAPHORE -o trap_semaphore trap.c -lpthread

trap_busywaiting : trap.c
	gcc -Wall -g -DD_BUSYWAITING -o trap_busywaiting trap.c -lpthread

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.out
	rm -rf *.dSYM
	rm pth_hello
	rm -f trap_*
