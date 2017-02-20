#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include <ctype.h>
#include "compress.h"


void * processWorker(fileMetaData *processData){
	compress_LOLS(processData->startPos, processData->endPos,processData->numParts,processData->fileName);

	free(processData->fileName);
	free(processData);
	return NULL;
}

int main(){
return 0; }
