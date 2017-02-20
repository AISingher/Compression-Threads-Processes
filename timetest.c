#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "compress.h"
#include <unistd.h>
#include <string.h>


void process_avg(int max_parts, int max_trials, long * averages) {
	int trial = 0, part = 1;
	
	for (;part<=max_parts;part++) {
		for(trial=0;trial < max_trials; trial++){
			long time = 0;
			struct timeval start, finish;
			
			gettimeofday(&start, NULL);
			
			compressR_LOLS("test.txt", part);
			
			gettimeofday(&finish, NULL);
			
			time += (long)(finish.tv_sec - start.tv_sec)*1000000L;
			time += (long)(finish.tv_usec - start.tv_usec);
			
			averages[part-1] += time;
		}
	}
	int i=0;
	for (;i<max_parts;i++){ 
		averages[i] /= max_trials;
		printf("process_averages %d=%ld microseconds\n",i,averages[i]);
	}
}

void thread_avg(int max_parts, int max_trials, long * averages) {
	int trial = 0, part = 1;
	
	for (;part<=max_parts;part++) {
		for(trial=0;trial < max_trials; trial++){
			long time = 0;
			struct timeval start, finish;
			
			gettimeofday(&start, NULL);
			
			compressT_LOLS("test.txt", part);
			
			gettimeofday(&finish, NULL);
			
			time += (long)(finish.tv_sec - start.tv_sec)*1000000L;
			time += (long)(finish.tv_usec - start.tv_usec);
			
			averages[part-1] += time;
		}
	}
	int i=0;
	for (;i<max_parts;i++){ 
		averages[i] /= max_trials;
		printf("thread_averages %d=%ld microseconds\n",i,averages[i]);
	}
}

int main(int argc, char ** argv){
	int trials=10;
	int parts = 1;
	long process_averages[parts];
	long thread_averages[parts];
	int f=0;
	for(;f<parts;f++){
		process_averages[f] = 0;
		thread_averages[f] = 0;
	}
	process_avg(parts,trials,process_averages);
	thread_avg(parts,trials,thread_averages);
	
	return 0;	
} 
