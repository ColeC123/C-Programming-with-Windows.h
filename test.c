#include "Buffer.h"
#include "ConsoleFunctions.h"
#include "DynamicMemory.h"
#include "stdio.h"
#include "timeapi.h"
#include "windows.h"

// Compilation command "gcc test.c -I C:\Windows\System32 -l winmm -o test"
// In order for timeapi.h to work, you have to link to winmm.dll file, which is stored in the System32 file
// Trying to compile without winmm.dll will result in a linker error
// This is because the function definitions for timapi.h are specified in winmm.dll
//  -L specifies library path, and -l specifies the specific library to be included

enum directions { up = 0,
                  down = 1,
                  left = 2,
                  right = 3 };

typedef struct Snake {
    Vector2* position;
    Vector2 head;
    enum directions direction;
    int body_size;
} Snake;

static const Snake default_snake = {
    .position = NULL,
    .head = {.X = 0, .Y = 0},
    .direction = up,
    .body_size = 0,
    // body_size refers to the amount of trailing sections on the snake, the head is treated seperately
};

int main(void) {
    Buffer buffer = default_buffer;
    define_buffer(&buffer);

    Snake snake = default_snake;

    hide_cursor();
    system("cls");

    DWORD past_time, curr_time;
    curr_time = timeGetTime();
    past_time = curr_time;

    int movement_speed = 170;

    int vertical_speed = 200;
    int horizontal_speed = 200;

    Vector2 berry = {
        .X = (timeGetTime() % (buffer.dimensions.X - 2)) + 1,
        .Y = (timeGetTime() % (buffer.dimensions.Y - 2)) + 1,
    };

    enum directions past_direction = left;

    while (1) {
        if (GetKey('q')) {
            free(buffer.string.contents);
            free(snake.position);
            show_cursor();
            return 0;
        }

        if (GetKey('w') && snake.direction != down) {
            snake.direction = up;
            movement_speed = vertical_speed;
        }

        if (GetKey('s') && snake.direction != up) {
            snake.direction = down;
            movement_speed = vertical_speed;
        }

        if (GetKey('a') && snake.direction != right) {
            snake.direction = left;
            movement_speed = horizontal_speed;
        }

        if (GetKey('d') && snake.direction != left) {
            snake.direction = right;
            movement_speed = horizontal_speed;
        }

        past_direction = snake.direction;

        curr_time = timeGetTime();
        //This if statement contains almost all of the game logic
        if (curr_time - past_time > movement_speed) {
            past_time = curr_time;

        if (snake.body_size > 0) {
            shift_right(snake.position, snake.body_size);
            snake.position[0] = snake.head;
        }

            switch (snake.direction) {
                case up:
                    snake.head.Y--;
                    break;
                case down:
                    snake.head.Y++;
                    break;
                case left:
                    snake.head.X--;
                    break;
                case right:
                    snake.head.X++;
                    break;
                default:
                    snake.head.X++;
            }

            if (snake.head.Y < 1) {
                snake.head.Y = buffer.dimensions.Y - 2;
            } else if (snake.head.Y >= buffer.dimensions.Y - 1) {
                snake.head.Y = 1;
            }

            if (snake.head.X < 1) {
                snake.head.X = buffer.dimensions.X - 2;
            } else if (snake.head.X >= buffer.dimensions.X - 1) {
                snake.head.X = 1;
            }

            if (snake.head.X == berry.X && snake.head.Y == berry.Y) {
                snake.body_size++;
                berry = (Vector2){
                    .X = (timeGetTime() % (buffer.dimensions.X - 2)) + 1,
                    .Y = (timeGetTime() % (buffer.dimensions.Y - 2)) + 1,
                };

                Vector2* temp = (Vector2*)realloc(snake.position, snake.body_size * sizeof(Vector2));
                if (temp == NULL) {
                    exit(1);
                } else {
                    snake.position = temp;
                    snake.position[snake.body_size - 1] = (Vector2){.X = 0, .Y = 0};
                }
            }

            for (int i = 0; i < snake.body_size; i++) {
                if (snake.head.X == snake.position[i].X && snake.head.Y == snake.position[i].Y) {
                    system("cls");
                    show_cursor();
                    free(snake.position);
                    free(buffer.string.contents);
                    gotoXY(10, 6);
                    printf("GameOver");
                    return 0;
                }
            }

            clear_buffer(&buffer);

            for (int i = 0; i < buffer.dimensions.X; i++) {
                set_buffer_value(&buffer, (Vector2){.X = i, .Y = 0}, '#');
                set_buffer_value(&buffer, (Vector2){.X = i, .Y = buffer.dimensions.Y - 1}, '#');
            }

            for (int i = 1; i < buffer.dimensions.Y - 1; i++) {
                set_buffer_value(&buffer, (Vector2){.X = 0, .Y = i}, '#');
                set_buffer_value(&buffer, (Vector2){.X = buffer.dimensions.X - 1, .Y = i}, '#');
            }

            set_buffer_value(&buffer, snake.head, 'S');

            set_buffer_value(&buffer, berry, 'O');

            for (int i = 0; i < snake.body_size; i++) {
                set_buffer_value(&buffer, snake.position[i], '#');
            }

            gotoXY(0, 0);

            printf("%s", buffer.string.contents);
        }
    }

    free(buffer.string.contents);
    free(snake.position);
    show_cursor();
    return 0;
}