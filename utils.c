#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

char *readFile(const char *filename){
    FILE *fptr = fopen(filename, "rb");
    if(!fptr) return NULL;

    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if(!buffer){
        fclose(fptr);
        return NULL;
    }

    fread(buffer, 1, size, fptr);
    buffer[size] = '\0';

    fclose(fptr);
    return buffer;
}