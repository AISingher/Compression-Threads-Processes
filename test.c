#include <stdio.h>
#include <stdlib.h>
#include "compress.h"

int main(int argc, char ** argv){

	char * fileName = (char*)malloc(50);
	int numParts;
	int option= 0;

	printf("1:Compress by processes\n");
	printf("2:Compress by threads\n");

	scanf("%d", &option);


	if(option==1){//compressR 
		printf("Enter filename followed by number of parts.\n");
		scanf("%s %d", fileName,&numParts);
		compressR_LOLS(fileName, numParts);
	}
	else if(option==2){//compressT
		printf("Enter filename followed by number of parts.\n");
		scanf("%s %d", fileName,&numParts);
		compressT_LOLS(fileName, numParts);
	}
	free(fileName);
	return 0;		
} 
