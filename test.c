#include "Buffer.h"
#include "ConsoleFunctions.h"
#include "DynamicMemory.h"
#include "stdio.h"
#include "windows.h"
#include "timeapi.h"

//Compilation command "gcc test.c -L C:\Windows\System32 -l winmm -o test"
//In order for timeapi.h to work, you have to link to winmm.dll file, which is stored in the System32 file
//Trying to compile without winmm.dll will result in a linker error
//This is because the function definitions for timapi.h are specified in winmm.dll
// -L specifies library path, and -l specifies the specific library to be included

enum directions { up = 0,
                  down = 1,
                  left = 2,
                  right = 3 };

typedef struct Snake {
    IntVector X;
    IntVector Y;
    enum directions direction;
} Snake;

static const Snake default_snake = {
    .X = {.contents = NULL, .datasize = 0, .memsize = 0},
    .Y = {.contents = NULL, .datasize = 0, .memsize = 0},
    .direction = up,
};

int main(void) {
    Buffer buffer = default_buffer;
    define_buffer(&buffer);

    Snake snake = default_snake;
    append_int(&snake.X, 5);
    append_int(&snake.Y, 5);

    hide_cursor();
    system("cls");

    DWORD past_time, curr_time;
    curr_time = timeGetTime();
    past_time = curr_time;

    while (1) {
        gotoXY(0, 0);

        if (GetKey('q')) {
            free(buffer.string.contents);
            return 0;
        }

        clear_buffer(&buffer);

        if (GetKey('w')) {
            snake.direction = up;
        }

        if (GetKey('s')) {
            snake.direction = down;
        }

        if (GetKey('a')) {
            snake.direction = left;
        }

        if (GetKey('d')) {
            snake.direction = right;
        }

        curr_time = timeGetTime();
        if (curr_time - past_time > 135) {
            past_time = curr_time;
            switch (snake.direction) {
                case up:
                    snake.Y.contents[0]--;
                    break;
                case down:
                    snake.Y.contents[0]++;
                    break;
                case left:
                    snake.X.contents[0]--;
                    break;
                case right:
                    snake.X.contents[0]++;
                    break;
                default:
                    snake.X.contents[0]++;
            }

            if (snake.Y.contents[0] < 0) {
                snake.Y.contents[0] = buffer.dimensions.Y - 1;
            } else if (snake.Y.contents[0] >= buffer.dimensions.Y) {
                snake.Y.contents[0] = 0;
            }

            if (snake.X.contents[0] < 0) {
                snake.X.contents[0] = buffer.dimensions.X - 1;
            } else if (snake.X.contents[0] >= buffer.dimensions.X) {
                snake.X.contents[0] = 0;
            }
        }

        set_buffer_value(&buffer, (Vector2){.X = snake.X.contents[0], .Y = snake.Y.contents[0]}, '#');

        printf("%s", buffer.string.contents);
    }

    free(buffer.string.contents);
    return 0;
}