#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "compress.h"
#include <sys/time.h>

    void * compressT_worker_LOLS(fileMetaData * threadData) {

    	compress_LOLS(threadData->startPos, threadData->endPos,threadData->numParts,threadData->fileName);

    	free(threadData->fileName);
    	free(threadData);

    	return 0;
    }

    void compressT_LOLS(char * fileName, int parts) {
	//error checking code
	if (fileName == NULL) { //empty or invalid string input
		fprintf(stderr,"Error: Invalid filename.\n");
		exit(EXIT_FAILURE);
	}

	if (parts < 1) { //0 or negative parts
		fprintf(stderr,"Error: Invalid number of parts.\n");
		exit(EXIT_FAILURE);
	}

	FILE * fp = fopen(fileName, "r");

	if (fp == NULL) {//empty file or nonexistent
		fprintf(stderr,"Error: Could not access file.\n");
		exit(EXIT_FAILURE);
	}
    //get number of chars in input file
	fseek(fp, 0, SEEK_END);
	int fileChars = ftell(fp);
	fclose(fp);

	if (parts > fileChars) {//compress file into more parts than chars
		fprintf(stderr,"Error: More parts than characters in file.\n");
		exit(EXIT_FAILURE);
	}
	
	int threadChars = fileChars / parts;
	int remainderChars = fileChars % parts;
//	printf("check chars in file %d check chars per thread %d check remainderChars %d \n",fileChars,threadChars,remainderChars);
	int i,temp;
	temp=0;
	pthread_t threadID;
	pthread_t tempThreadID[parts];
    //thread spawning code through compressT_worker_LOLS to compress parts
	for (i=0;i<parts;i++) {
		
		fileMetaData * data = malloc(sizeof(fileMetaData)); 
		data->fileName = malloc(strlen(fileName)+1);
		strcpy(data->fileName,fileName);

		data->numParts= parts == 1 ? -1 : i; //else >1 parts
		data->startPos= i == 0 ? 0 : temp;
		data->endPos= i == 0 ? threadChars + remainderChars -1 : threadChars + temp -1; //add remainder to chars per part, else add chars per part >1parts

		pthread_create(&threadID, NULL, (void * (*)(void *))compressT_worker_LOLS, data);

		tempThreadID[i] = threadID;
		temp = data->endPos + 1;
	}
	
	i=0;

	for(i=0;i<parts;i++){//wait on each thread
		pthread_join(tempThreadID[i],NULL);
	}
}
