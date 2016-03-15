#!/bin/sh

#PBS -N ivyBridge_pthreads
#PBS -l walltime=00:01:00
#PBS -q mei 
#PBS -m abe
#PBS -M carlos.sa01@gmail.com
#PBS -l nodes=1:r641:ppn=32

max_threads=16
max_sample=5
num_threads=1

a=2
b=242
n=12

cd /home/a59905/ESC_TP2/src/
module purge
module load gnu/4.9.3

echo "Executing Ex1 for pth_hello..."

echo "NThreads,Average_Time_Per_Thread(ms),Time(ms)" >> CSV/pth_hello_output.csv

echo "Sequential pth_hello..."
for ((sample=1; sample <= $max_sample; sample++ ))
do	
	./pth_hello $num_threads >> CSV/pth_hello_output.csv
done

echo "Parallel pth_hello using threads...."
for (( num_threads=2; num_threads <= $max_threads; num_threads+=2 ))
do
	for ((sample=1; sample <= $max_sample; sample++ ))
	do	
		./pth_hello $num_threads >> CSV/pth_hello_output.csv
	done
done




echo "Executing Ex2..."

echo "MutualExclusion,NThreads,val_a,val_b,nRectangles/Sub-Intervals,Result,Time(ms)" >> CSV/trap_mutex_output.csv
echo "MutualExclusion,NThreads,val_a,val_b,nRectangles/Sub-Intervals,Result,Time(ms)" >> CSV/trap_semaphore_output.csv
echo "MutualExclusion,NThreads,val_a,val_b,nRectangles/Sub-Intervals,Result,Time(ms)" >> CSV/trap_busywaiting_output.csv

echo "Sequential MUTEX | SEMAPHORE | BUSY-WAITING..."
for ((sample=1; sample <= $max_sample; sample++ ))
do	
	./trap_mutex 1 $a $b $n >> CSV/trap_mutex_output.csv
	./trap_semaphore 1 $a $b $n >> CSV/trap_semaphore_output.csv
	./trap_busywaiting 1 $a $b $n >> CSV/trap_busywaiting_output.csv
done

echo "Parallel MUTEX | SEMAPHORE | BUSY-WAITING...."
for (( num_threads=2; num_threads <= $max_threads; num_threads+=2 ))
do
	for ((sample=1; sample <= $max_sample; sample++ ))
	do	
		./trap_mutex $num_threads $a $b $n >> CSV/trap_mutex_output.csv
		./trap_semaphore $num_threads $a $b $n >> CSV/trap_semaphore_output.csv
		./trap_busywaiting $num_threads $a $b $n >> CSV/trap_busywaiting_output.csv
	done
done
echo "Done! All CSV's generated!...."
