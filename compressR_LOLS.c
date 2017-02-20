#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>
#include <sys/wait.h>
#include "compress.h"

void * processWorker(fileMetaData *processData){
	compress_LOLS(processData->startPos, processData->endPos,processData->numParts,processData->fileName);

	free(processData->fileName);
	free(processData);
	return NULL;
}

void compressR_LOLS(char * fileName, int parts) {
	//error checking
	if (fileName == NULL) {
		fprintf(stderr,"Error: Invalid filename.\n");
		exit(EXIT_FAILURE);
	}

	if (parts < 1) {
		fprintf(stderr,"Error: Invalid number of parts.\n");
		exit(EXIT_FAILURE);
	}

	FILE * fp = fopen(fileName, "r");

	if (fp == NULL) {
		fprintf(stderr,"Error: Could not access file.\n");
		exit(EXIT_FAILURE);
	}

	fseek(fp, 0, SEEK_END);
	int fileChars = ftell(fp);
	fclose(fp);

	if (parts > fileChars) {
		fprintf(stderr,"Error: More parts than characters in file.\n");
		exit(EXIT_FAILURE);
	}



	//process spawning
	int processChars = fileChars / parts;
	int remainderChars = fileChars % parts;
	//printf("check chars in file %d check chars per process %d check remainderChars %d \n",fileChars,processChars,remainderChars);
//	int prev = 0;

	

        pid_t pids[parts];
//	pid_t process_id;
	
	int i=0;
  	int * shared = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0); 
 
	for (i=0;i<parts;i++) {
	
	
	pids[i] = fork();

        if(pids[i] < 0){

        exit(1);
        }

        else if(pids[i] == 0){
		fileMetaData * data = malloc(sizeof(fileMetaData));
		data->fileName = malloc(strlen(fileName)+1);
		strcpy(data->fileName,fileName);

		data->numParts= parts == 1 ? -1 : i;
 
		if (i == 0){
			data->startPos = 0;
		//	printf("Process char : %d \n remainderChars %d\n", processChars, remainderChars);
			data->endPos = processChars + remainderChars - 1;
		
		} else {
		//	data->startPos = prev;
		//	data->endPos = processChars + prev - 1;
			data->startPos = *shared;
			data->endPos = processChars + (*shared) - 1;
		}
			
		//processWorker(data);
		
		
		//prev = data->endPos + 1;
		*shared = data->endPos + 1;		
		
		processWorker(data);
		exit(0);

			
	
       
}
	
	else{  wait(NULL);  }
	
	}	
}

