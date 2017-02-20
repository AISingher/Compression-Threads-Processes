#ifndef COMPRESS_H
#define COMPRESS_H

typedef struct fileMetaData {
	int startPos;
	int endPos;
	int numParts;
	char * fileName;
} fileMetaData;

void compress_LOLS(int, int, int, char *);
void compressT_LOLS(char *, int);
void * threadWorker(fileMetaData *);
void compressR_LOLS(char *, int);
void * processWorker(fileMetaData *);
#endif
