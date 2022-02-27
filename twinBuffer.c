#include "twinBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"

const char EOF_CHAR = '\0';

void printBufState(TwinBuffer* tbuf){
    int lbuf = tbuf->lexemeBegin.bufNo;
    int fbuf = tbuf->forward.bufNo;
    
    char* lstr = (lbuf == tbuf->first) ? "First Buffer" : "Second Buffer";
    char* fstr = (fbuf == tbuf->first) ? "First Buffer" : "Second Buffer";

    int lin = tbuf->lexemeBegin.index;
    int fin = tbuf->forward.index;

    LOG("LexemeBegin = {%s, %d}\t Forward = {%s, %d}\n", lstr, lin, fstr, fin);
}

void swap(int* a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int initTwinBuffer(TwinBuffer* tbuf, char* pathToSrc){
    tbuf->first = 0;
    tbuf->second = 1;

    tbuf->forward = (BufferHead) {tbuf->first, -1};
    tbuf->lexemeBegin = (BufferHead) {tbuf->first, 0};

    tbuf->fptr = fopen(pathToSrc, "r");
    if(tbuf->fptr == NULL){
        printf("ERROR!! Cannot open file %s\n", pathToSrc);
        return -1;
    }
    
    fillBuffer(tbuf, tbuf->first);
    return 0;
}

int isEndOfFirstBuffer(TwinBuffer* tbuf){
    return tbuf->forward.index == BUFFER_SIZE - 1 && tbuf->forward.bufNo == tbuf->first;
}

int isEndOfSecondbuffer(TwinBuffer* tbuf){
    return tbuf->forward.index == BUFFER_SIZE - 1 && tbuf->forward.bufNo == tbuf->second;
}

int nextChar(TwinBuffer* tbuf, char* ch){
    LOG("[Last read char pos = %d]\n", tbuf->forward.index);
    if(isEndOfFirstBuffer(tbuf)){
        LOG("First buffer full; Filling second buffer\n");
        if(!tbuf->secondBufFilled)
            fillBuffer(tbuf, tbuf->second);
        tbuf->forward = (BufferHead) {tbuf->second, -1};
    }
    else if(isEndOfSecondbuffer(tbuf)){
        printf("ERROR!! Attempt to call nextChar beyond end of second\n");
        return -1;
    }
    tbuf->forward.index++;
    *ch = tbuf->buf[tbuf->forward.bufNo][tbuf->forward.index];
    return 0;
}

int resetBegin(TwinBuffer* tbuf, int rewindCount){
    if(rewindCount > BUFFER_SIZE){
        printf("ERROR!! Cannot rewind buffer greater than BUFFER_SIZE(%d)\n", BUFFER_SIZE);
        return -1;
    }

    int fIndex = tbuf->forward.index;
    int fBufNo = tbuf->forward.bufNo;

    fIndex = fIndex - rewindCount + 1;
    if(fIndex < 0 && fBufNo == tbuf->first){
        printf("ERROR!! Cannot rewind buffer to before beginning of first buffer\n");
        return -1;
    }
    else if(fIndex < 0){
        tbuf->secondBufFilled = 1;
        fIndex = BUFFER_SIZE + fIndex;
        tbuf->lexemeBegin = (BufferHead) {tbuf->first, fIndex};
        tbuf->forward = (BufferHead) {tbuf->first, fIndex - 1};
    }
    else if(fBufNo == tbuf->second){
        swap(&(tbuf->first), &(tbuf->second));
    }
    tbuf->secondBufFilled = 0;
    tbuf->lexemeBegin = (BufferHead) {tbuf->first, fIndex};
    tbuf->forward = (BufferHead) {tbuf->first, fIndex - 1};
    
    LOG("[Registerd lexeme end.]\n");
    printBufState(tbuf);
    return 0;
}

int fillBuffer(TwinBuffer* tbuf, int index){
    int bytesRead = fread(tbuf->buf[index], sizeof(char), BUFFER_SIZE, tbuf->fptr);

    if(bytesRead < BUFFER_SIZE && !feof(tbuf->fptr)){
        printf("ERROR!! Error reading from source file.");
        return -1;
    }
    else if(bytesRead < BUFFER_SIZE){
        tbuf->buf[index][bytesRead] = EOF_CHAR;
    }
    return bytesRead;
}

int closeBuffer(TwinBuffer* tbuf){
    return fclose(tbuf->fptr);
}