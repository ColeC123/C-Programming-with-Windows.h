#ifndef BUFFER_H
#define BUFFER_H
#include "DynamicMemory.h"

typedef struct Vector2 {
    int X;
    int Y;
} Vector2;

typedef struct Buffer {
    string string;
    Vector2 dimensions;
} Buffer;

//Sets up the default values for the buffer
//This is most done to make the initialization process easier with the associated functions
static const Buffer default_buffer = {
    .string = {.contents = NULL, .memsize = 0, .datasize = 0},
    .dimensions = {.X = 30, .Y = 12},
};

//Creates a buffer variable and creats a blank buffer
void define_buffer(Buffer* buffer) {
    free(buffer->string.contents);
    buffer->string = default_string;
    for (int i = 0; i < buffer->dimensions.Y; i++) {
        for (int j = 0; j < buffer->dimensions.X; j++) {
            //This creates the blank part
            append_char(&buffer->string, ' ');
        }
        //This is the newline character since a 2D array is not being used
        append_char(&buffer->string, '\n');
    }
    //This is the null terminator character, which lets printf know when the string ends
    append_char(&buffer->string, '\0');
}

void set_buffer_value(Buffer* buffer, Vector2 position, char input) {
    if (position.X < buffer->dimensions.X && position.Y < buffer->dimensions.Y) {
        //The formula inside the brackets below accesses the correct arrray value given a 2D point
        //This allows it to be worked with as if it were a 2D array, when in reality it is not
        //This makes the freeing of memory a lot easier
        buffer->string.contents[(position.X % buffer->dimensions.X) + ((buffer->dimensions.X + 1) * position.Y)] = input;
    }
}

char get_buffer_value(Buffer* buffer, Vector2 position) {
    if (position.X < buffer->dimensions.X && position.Y < buffer->dimensions.Y) {
        //The same formula as used above, but this time it returns the value at that point
        return buffer->string.contents[(position.X % buffer->dimensions.X) + ((buffer->dimensions.X + 1) * position.Y)];
    }
}

//Initializes all of the available drawing points (not null terminator or newline character) to be blank
void clear_buffer(Buffer* buffer) {
    for (int i = 0; i < buffer->dimensions.Y; i++) {
        for (int j = 0; j < buffer->dimensions.X; j++) {
            set_buffer_value(buffer, (Vector2){.X = j, .Y = i}, ' ');
        }
    }
}

void shift_right(Vector2* vec, int size) {
    //The use of temporary variables makes it possible to avoid having to create a copy of the array
    Vector2 temp_one = (Vector2){.X = vec[0].X, .Y = vec[0].Y};
    for (int i = 1; i < size; i++) {
        Vector2 temp_two = (Vector2){.X = vec[i].X, .Y = vec[i].Y};
        vec[i] = (Vector2){.X = temp_one.X, .Y = temp_one.Y};
        temp_one = (Vector2){.X = temp_two.X, .Y = temp_two.Y};
    }
    vec[0] = (Vector2){.X = 0, .Y = 0};
}

#endif