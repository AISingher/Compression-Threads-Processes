#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "compress.h"
#include <sys/time.h>
#define AddOutputChar fputc(outputChar, output);

void compress_LOLS(int start, int end, int numParts, char * fileName) { //LOLS compression into compressed files to be used later in thread spawning and proccess worker

//output or compressed file name,memory code

    int partsLength;//length of numParts integer
    int i=0;
    int numDigits=1;
    int temp=numParts;
    char *outputName;//compressed file name
    FILE * input, *output; //input is the uncompressed file to read from, output is a compressed file to write to

    while(temp/10!=0){//get length of an int, divide int by 10 add 1 until cannot, ie 100 returns 2
    	temp/=10;
    	numDigits++;
    }
   // printf("%d",numDigits);

    char partsLengthTemp[numDigits];//string version of number of parts length
    sprintf(partsLengthTemp,"%d",numParts); //for length of number of parts
    partsLength=strlen(partsLengthTemp);

    if(numParts<0){
		outputName = malloc(strlen(fileName) + 1 + 5); 
        memset(outputName,0,strlen(fileName) + 1 + 5);
         strcat(outputName, "_LOLS");
    }
    else{
    	outputName = malloc(strlen(fileName) + 1 + 5 + partsLength); //space for filename + null terminal+ "_LOLS" + number of parts size 
        memset(outputName,0,strlen(fileName) + 1 + 5 + partsLength);
         strcat(outputName, "_LOLS");
    }
    

    while(fileName[i] != '\0'){// replace . with _ for .txt file

    	if (fileName[i] == '.') {
    		outputName[i] = '_';
    	} else {
    		outputName[i] = fileName[i];
    	}

    	i++;
    }

    strcat(outputName, "_LOLS");//add _LOLS to compressed name

    if (numParts >=0) {
    	char partString[partsLength];
    	sprintf(partString, "%d", numParts);
    	strcat(outputName, partString);
    }

    input = fopen(fileName, "r");//read from uncompressed file

    if(access(outputName,F_OK)!=-1){//compressed files, in name, already exists
    	printf("warning: %s already exists! Overwriting...\n",outputName);
    }	

    output = fopen(outputName, "w+");//write and overwrite permissions for compressed file(s)
    free(outputName);
    char inputChar,outputChar;
    int tempStart = start;
    int charCount = 1;
    fseek(input, start, SEEK_SET);//start of desired range
    outputChar = fgetc(input);//characters in desired range

    while(outputChar!=EOF && tempStart <=end && !isalpha(outputChar)){ //first character in file non-alphabetical, loop until alphabetical char found
    	if(tempStart==end){
    		tempStart++; break;}    	
    	outputChar=fgetc(input);
    	tempStart++;
    	}

    //LOLS compression part code
    //goes from start to end of desired range in compressed file
    // adding desired output chars into the compressed file and ensuring the're alphabetical
    	for (i = 0; tempStart <= end && outputChar != EOF; i++) {
    		inputChar = fgetc(input);

    		if(!isalpha(inputChar)){
        	//continue if non-alphabetical character
    		}
    		else if(inputChar == outputChar) {
    			charCount++;
    		} 
    		else {// add character to output file
    			if(charCount == 1){
    				AddOutputChar; 
    			}
    			else if(charCount == 2){
    				AddOutputChar;
    				AddOutputChar; 
    			}
    			else{
    				fprintf(output, "%d", charCount);
    				AddOutputChar; 
    			}
    			outputChar = inputChar;
    			charCount = 1;    			
    		}
    		tempStart++;
    	}
    	if (isalpha(inputChar)){
    		charCount--;
    	}
    	if (charCount > 0 && isalpha(outputChar)) { //make sure we got all characters
    		if(charCount == 1){
    				AddOutputChar; 
    			}
    			else if(charCount == 2){
    				AddOutputChar;
    				AddOutputChar; 
    			}
    			else{
    				fprintf(output, "%d", charCount);
    				AddOutputChar; 
    			}
    	}

    	fclose(input);
    	fclose(output);
    }