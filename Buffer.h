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

static const Buffer default_buffer = {
    .string = {.contents = NULL, .memsize = 0, .datasize = 0},
    .dimensions = {.X = 30, .Y = 12},
};

void define_buffer(Buffer* buffer) {
    free(buffer->string.contents);
    buffer->string = default_string;
    for (int i = 0; i < buffer->dimensions.Y; i++) {
        for (int j = 0; j < buffer->dimensions.X; j++) {
            append_char(&buffer->string, ' ');
        }
        append_char(&buffer->string, '\n');
    }
    append_char(&buffer->string, '\0');
}

void set_buffer_value(Buffer* buffer, Vector2 position, char input) {
    if (position.X < buffer->dimensions.X && position.Y < buffer->dimensions.Y) {
        buffer->string.contents[(position.X % buffer->dimensions.X) + ((buffer->dimensions.X + 1) * position.Y)] = input;
    }
}

char get_buffer_value(Buffer* buffer, Vector2 position) {
    if (position.X < buffer->dimensions.X && position.Y < buffer->dimensions.Y) {
        return buffer->string.contents[(position.X % buffer->dimensions.X) + ((buffer->dimensions.X + 1) * position.Y)];
    }
}

void clear_buffer(Buffer* buffer) {
    for (int i = 0; i < buffer->dimensions.Y; i++) {
        for (int j = 0; j < buffer->dimensions.X; j++) {
            set_buffer_value(buffer, (Vector2){.X = j, .Y = i}, ' ');
        }
    }
}

#endif