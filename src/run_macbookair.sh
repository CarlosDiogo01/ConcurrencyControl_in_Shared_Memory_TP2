#!/bin/bash

max_threads=16
max_sample=5
numthreads=1

a=2
b=242
n=12

echo "Executing Ex1 for pth_hello..."

echo "NThreads,Average_Time_Per_Thread(ms),Time(ms)" >> CSV/pth_hello_output.csv

echo "Sequential pth_hello..."
for ((sample=1; sample <= $max_sample; sample++ ))
do	
	./pth_hello 1 >> CSV/pth_hello_output.csv
done

echo "Parallel pth_hello using threads...."
for (( numthreads=2; numthreads <= $max_threads; numthreads+=2 ))
do
	for ((sample=1; sample <= $max_sample; sample++ ))
	do	
		./pth_hello $numthreads >> CSV/pth_hello_output.csv
	done
done




echo "Executing Ex2..."

echo "MutualExclusion,NThreads,val_a,val_b,nRectangles/Sub-Intervals,Result,Time(ms)" >> CSV/trap_mutex_output.csv
echo "MutualExclusion,NThreads,val_a,val_b,nRectangles/Sub-Intervals,Result,Time(ms)" >> CSV/trap_semaphore_output.csv
echo "MutualExclusion,NThreads,val_a,val_b,nRectangles/Sub-Intervals,Result,Time(ms)" >> CSV/trap_busywaiting_output.csv

echo "Sequential MUTEX | SEMAPHORE | BUSY-WAITING..."
for (( sample=1; sample <= $max_sample; sample++ ))
do	
	./trap_mutex 1 $a $b $n >> CSV/trap_mutex_output.csv
	./trap_semaphore 1 $a $b $n >> CSV/trap_semaphore_output.csv
	./trap_busywaiting 1 $a $b $n >> CSV/trap_busywaiting_output.csv
done

echo "Parallel MUTEX | SEMAPHORE | BUSY-WAITING...."
for (( numthreads=2; numthreads <= $max_threads; numthreads+=2 ))
do
	for ((sample=1; sample <= $max_sample; sample++ ))
	do	
		./trap_mutex $numthreads $a $b $n >> CSV/trap_mutex_output.csv
		./trap_semaphore $numthreads $a $b $n >> CSV/trap_semaphore_output.csv
		./trap_busywaiting $numthreads $a $b $n >> CSV/trap_busywaiting_output.csv
	done
done
echo "Done! All CSV's generated!...."
