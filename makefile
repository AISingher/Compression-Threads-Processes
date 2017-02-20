CC      =       gcc
FLAGS   =       -Wall -g
COMPILE =       $(CC) $(FLAGS)

all: compressT_LOLS compressR_LOLS test timetest

compressT_LOLS: compressT_LOLS.c compress_LOLS.c compress.h
	$(COMPILE) -pthread -c compress_LOLS.c compressT_LOLS.c

compressR_LOLS: compressR_LOLS.c compressR_worker_LOLS.c compress_LOLS.c compress.h
	$(COMPILE) -c compress_LOLS.c compressR_LOLS.c
	$(COMPILE) compress_LOLS.c compressR_worker_LOLS.c -o compressR_worker_LOLS 

test: test.c compress_LOLS.o compressT_LOLS.o compress.h
	$(COMPILE) -pthread test.c compress_LOLS.o compressT_LOLS.o compressR_LOLS.o -o test
	
timetest:
	$(COMPILE)  -pthread compressT_LOLS.c compress_LOLS.c compressR_LOLS.c timetest.c -o timetest

clean:
	rm -rf *.o compressR_worker_LOLS test
