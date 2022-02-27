#ifndef _TWIN_BUFFER_
#define _TWIN_BUFFER_

#include <stdio.h>

#define BUFFER_SIZE 256
extern const char EOF_CHAR;
typedef struct{
    int bufNo;
    int index;
} BufferHead;

typedef struct{
    char buf[2][BUFFER_SIZE];
    int first;
    int second;

    int secondBufFilled; // indicates whether second buffer is already filled

    BufferHead forward; // points to last read character
    BufferHead lexemeBegin; // points to begining of lexeme

    FILE* fptr;
} TwinBuffer;

/*
    Initializes the twin buffer and open the file specified by pathToSrc 
    as the input file for the buffer 
*/
int initTwinBuffer(TwinBuffer* tbuf, char* pathToSrc);

/*
    Gets next character in the file. Character returned is EOF if file has 
    been read fully. Reloads second buffer if read beyond first buffer.
    Returns -1 if attempted to call beyond second buffer.
*/
int nextChar(TwinBuffer* tbuf, char* ch);

/*
    Acknowledges the end of current lexeme and re-initializes lexemeBegin by
    rewinding the forward pointer by rewindCount times. Returns -1
    if rewind occurs beyond the beginning of first buffer
*/
int resetBegin(TwinBuffer* tbuf, int rewindCount);

/*
    Fills buffer by reading BUFFER_SIZE bytes from 
    file. Returns -1 on error else, bytes read.
*/
int fillBuffer(TwinBuffer* tbuf, int index);

/*
    Closes fptr
*/
int closeBuffer(TwinBuffer* tbuf);

#endif


