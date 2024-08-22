#ifndef DYNAMICMEMORY_H
#define DYNAMICMEMORY_H
#include "stdio.h"
#include "stdlib.h"

typedef struct string {
    char* contents;
    int memsize;
    int datasize;
} string;

static const string default_string = {.contents = NULL, .memsize = 0, .datasize = 0};

void append_char(string* string, char input) {
    if (string->datasize < string->memsize) {
        string->contents[string->datasize] = input;
        string->datasize++;

    } else {
        string->memsize++;
        char* test = (char*)realloc(string->contents, 2 * string->memsize * sizeof(char));

        if (test == NULL) {
            printf("\nRealloc Failed for char append. memsize is: %d\n", string->memsize);
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