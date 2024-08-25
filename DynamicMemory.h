#ifndef DYNAMICMEMORY_H
#define DYNAMICMEMORY_H
#include "stdio.h"
#include "stdlib.h"

typedef struct string {
    char* contents;
    int memsize;
    int datasize;
} string;

//The default string state. Functions defined below rely on these default values for initialization.
//When using realloc with a NULL pointer, it will act like malloc. This simplifies the code a bit.
static const string default_string = {.contents = NULL, .memsize = 0, .datasize = 0};

//Adds a character to the end of the char* part of the string struct
void append_char(string* string, char input) {
    //If the current position in the character array is less then the memory stored, simply increment the position in the character array
    //The position in the character array is stored in string->datasize
    if (string->datasize < string->memsize) {
        string->contents[string->datasize] = input;
        string->datasize++;

    } else {
        //memsize is incremented since the default memsize value for a string is 0
        //Using realloc with a size of 0 acts like free, so memsize is incremented to prevent a possible double freeing
        string->memsize++;

        //Double memroy size of array since copying values is expensive
        char* test = (char*)realloc(string->contents, 2 * string->memsize * sizeof(char));

        //Make Sure realoc didn't fail
        if (test == NULL) {
           exit(1);
        } else {
            string->contents = test;
            string->memsize *= 2;
            string->contents[string->datasize] = input;
            string->datasize++;
        }
    }
}

void append_string(string* to, char* from_c, string* from_s) {

    //if both from_c ad from_s are not NULL, then from_s will be appended

    //if both from_c and from_s are NULL, then nothing will happen

    if (from_s != NULL) {
        for (int i = 0; i < from_s->datasize; i++) {
            append_char(to, from_s->contents[i]);
        }
    } else if (from_c != NULL) {
        while (*from_c != '\0') {
            append_char(to, *from_c);
            from_c++;
        }
    }
}

#endif