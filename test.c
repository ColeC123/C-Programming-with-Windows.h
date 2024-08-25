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
//  -I specifies library path, and -l specifies the specific library to be included

enum directions { up = 0,
                  down = 1,
                  left = 2,
                  right = 3 };

typedef struct Snake {
    Vector2* positions;
    Vector2 head;
    enum directions direction;
    int body_size;
} Snake;

static const Snake default_snake = {
    .positions = NULL,
    .head = {.X = 0, .Y = 0},
    .direction = up,
    .body_size = 0,
    // body_size refers to the amount of trailing sections on the snake, the head is treated seperately
};

int PlayGame(void);

int EndGame(void);

int main(void) {
    PlayGame();
    return 0;
}

int PlayGame(void) {
    Buffer buffer = default_buffer;
    define_buffer(&buffer);

    Snake snake = default_snake;

    hide_cursor();
    system("cls");

    // Stores the past and current time in miliseconds (timeGetTime returns time in miliseconds)
    DWORD past_time, curr_time;
    curr_time = timeGetTime();
    past_time = curr_time;

    int movement_speed = 170;

    int vertical_speed = 200;
    int horizontal_speed = 200;

    // Default 'past direction' for snake. See reasoning for inclusion near the start of the while loop
    enum directions past_direction = left;

    // Initialize an array for the available berry positions (the places where the snake is not present)
    int available_berry_positions_size = (buffer.dimensions.Y - 2) * (buffer.dimensions.X - 2);
    Vector2* available_berry_positions = (Vector2*)malloc(available_berry_positions_size * sizeof(Vector2));

    for (int i = 1; i < buffer.dimensions.Y - 1; i++) {
        for (int j = 1; j < buffer.dimensions.X - 1; j++) {
            available_berry_positions[((j - 1) % (buffer.dimensions.X - 1)) + ((i - 1) * (buffer.dimensions.X - 2))] = (Vector2){.X = j, .Y = i};
        }
    }

    // Initialize the berry's first position
    Vector2 berry = {
        .X = available_berry_positions[timeGetTime() % available_berry_positions_size].X,
        .Y = available_berry_positions[timeGetTime() % available_berry_positions_size].Y,
    };

    while (1) {
        // Quits when q is pressed
        if (GetKey('q')) {
            free(available_berry_positions);
            free(buffer.string.contents);
            free(snake.positions);
            show_cursor();
            return 0;
        }

        // Input is decoupled from the logic loop so that the game is more responsive
        // Past direction is included to prevent player from being able to switch directions into snake body
        // Since input is detected faster than game logic, player could rapidly press keys to change positions illegally
        // This included turning head position to move towards the snake's body, causing game over
        if ((GetKey(wKey) || GetKey(upArrow)) && (past_direction != down)) {
            snake.direction = up;
            movement_speed = vertical_speed;
        }

        if ((GetKey(sKey) || GetKey(downArrow)) && (past_direction != up)) {
            snake.direction = down;
            movement_speed = vertical_speed;
        }

        if ((GetKey(aKey) || GetKey(leftArrow)) && (past_direction != right)) {
            snake.direction = left;
            movement_speed = horizontal_speed;
        }

        if ((GetKey(dKey) || GetKey(rightArrow)) && (past_direction != left)) {
            snake.direction = right;
            movement_speed = horizontal_speed;
        }

        curr_time = timeGetTime();

        // This if statement contains almost all of the game logic
        if (curr_time - past_time > movement_speed) {
            past_time = curr_time;

            past_direction = snake.direction;

            if (snake.body_size > 0) {
                // Moves older snake positions to the right on in the array
                // This is how the snake's body positions are determined
                shift_right(snake.positions, snake.body_size);
                snake.positions[0] = snake.head;
            }

            // This removes the available points for the berry to spawn so that the berry doesn't spawn inside of the snake.
            // This code specifically checks for the snake's body
            for (int i = 0; i < snake.body_size; i++) {
                for (int j = 0; j < available_berry_positions_size; j++) {
                    if (available_berry_positions[j].X == snake.positions[i].X && available_berry_positions[j].Y == snake.positions[i].Y) {
                        for (int k = j; k < available_berry_positions_size - 1; k++) {
                            // Shift everything left to fill the point that is to be removed
                            available_berry_positions[k] = available_berry_positions[k + 1];
                        }

                        // Decrement available berry positions in to accound for removed Vector2
                        available_berry_positions_size--;

                        // Decrement j since we moved the Vector2 value at j + 1 into j
                        // This means we need to check the value that was at j + 1 to make sure it isn't a part of the snake's body
                        j--;
                    }
                }
            }

            // A repeat of the algorithm above, but this time it removes the snake's head from the available berry positions array
            for (int i = 0; i < available_berry_positions_size; i++) {
                if (available_berry_positions[i].X == snake.head.X && available_berry_positions[i].Y == snake.head.Y) {
                    for (int j = i; j < available_berry_positions_size - 1; j++) {
                        available_berry_positions[j] = available_berry_positions[j + 1];
                    }
                    available_berry_positions_size--;
                    i--;
                }
            }

            // Update snake head position based on current keyinput
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

            // If the snake reaches the top or bottom, it is sent to the opposite side (top or bottom)
            if (snake.head.Y < 1) {
                snake.head.Y = buffer.dimensions.Y - 2;
            } else if (snake.head.Y >= buffer.dimensions.Y - 1) {
                snake.head.Y = 1;
            }

            // If the snake reaches the left or right side, it is sent to the opposite side (left or right)
            if (snake.head.X < 1) {
                snake.head.X = buffer.dimensions.X - 2;
            } else if (snake.head.X >= buffer.dimensions.X - 1) {
                snake.head.X = 1;
            }

            // Checks if snake head has collided with berry
            if (snake.head.X == berry.X && snake.head.Y == berry.Y) {
                snake.body_size++;
                berry = (Vector2){
                    // Time is used a cheap way for pseudo random number generation
                    // This time value is used to calculate a 'random' index into available_berry_positions array
                    .X = available_berry_positions[timeGetTime() % available_berry_positions_size].X,
                    .Y = available_berry_positions[timeGetTime() % available_berry_positions_size].Y,
                };

                Vector2* temp = (Vector2*)realloc(snake.positions, snake.body_size * sizeof(Vector2));

                // Check to make sure that realloc hasn't failed
                if (temp == NULL) {
                    exit(1);
                } else {
                    snake.positions = temp;
                    snake.positions[snake.body_size - 1] = (Vector2){.X = 0, .Y = 0};
                }
            }

            // If the snake's head collides with any part of its body, free memory, and end the game
            for (int i = 0; i < snake.body_size; i++) {
                if (snake.head.X == snake.positions[i].X && snake.head.Y == snake.positions[i].Y) {
                    system("cls");
                    free(snake.positions);
                    free(buffer.string.contents);
                    free(available_berry_positions);
                    EndGame();
                    return 0;
                }
            }

            clear_buffer(&buffer);

            // Draw Horizontal Border
            for (int i = 0; i < buffer.dimensions.X; i++) {
                set_buffer_value(&buffer, (Vector2){.X = i, .Y = 0}, '#');
                set_buffer_value(&buffer, (Vector2){.X = i, .Y = buffer.dimensions.Y - 1}, '#');
            }

            // Draw Vertical Border
            for (int i = 1; i < buffer.dimensions.Y - 1; i++) {
                set_buffer_value(&buffer, (Vector2){.X = 0, .Y = i}, '#');
                set_buffer_value(&buffer, (Vector2){.X = buffer.dimensions.X - 1, .Y = i}, '#');
            }

            set_buffer_value(&buffer, snake.head, 'S');

            set_buffer_value(&buffer, berry, 'O');

            for (int i = 0; i < snake.body_size; i++) {
                set_buffer_value(&buffer, snake.positions[i], '#');
            }

            gotoXY(0, 0);

            printf("%s", buffer.string.contents);

            // Resets the available berry positions to all points in the grid
            // This must be done since the snake's position updates each logic loop
            for (int i = 1; i < buffer.dimensions.Y - 1; i++) {
                for (int j = 1; j < buffer.dimensions.X - 1; j++) {
                    available_berry_positions[((j - 1) % (buffer.dimensions.X - 1)) + ((i - 1) * (buffer.dimensions.X - 2))] = (Vector2){.X = j, .Y = i};
                }
            }

            // Reset the size of the available_berry_positions to match the size of the grid
            available_berry_positions_size = (buffer.dimensions.Y - 2) * (buffer.dimensions.X - 2);
        }
    }

    free(available_berry_positions);
    free(buffer.string.contents);
    free(snake.positions);
    show_cursor();
    return 0;
}

int EndGame(void) {
    Buffer buffer = default_buffer;
    define_buffer(&buffer);

    gotoXY(0, 0);

    // Draw the horizontal border
    for (int i = 0; i < buffer.dimensions.X; i++) {
        set_buffer_value(&buffer, (Vector2){.X = i, .Y = 0}, '#');
        set_buffer_value(&buffer, (Vector2){.X = i, .Y = buffer.dimensions.Y - 1}, '#');
    }

    // Draw the vertical border
    for (int i = 1; i < buffer.dimensions.Y - 1; i++) {
        set_buffer_value(&buffer, (Vector2){.X = 0, .Y = i}, '#');
        set_buffer_value(&buffer, (Vector2){.X = buffer.dimensions.X - 1, .Y = i}, '#');
    }

    printf("%s", buffer.string.contents);

    gotoXY(buffer.dimensions.X / 2 - 5, buffer.dimensions.Y / 2);

    printf("Game Over");

    gotoXY(0, buffer.dimensions.Y + 2);

    while (1) {
        if (GetKey(qKey)) {
            free(buffer.string.contents);
            ShowCursor(true);
            return 0;
        }
    }

    return 0;
}