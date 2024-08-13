#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "windows.h"

void gotoXY(int column, int row);

void reset_buffer(char** buffer, int height, int width);

void hide_cursor(void);

bool GetKey(char input);

char** init_buffer(int height, int width);

void free_buffer(char** buffer, int height);

int main(void) {
    int height = 12;
    int width = 30;

    char** buffer = init_buffer(height, width);

    hide_cursor();
    system("cls");

    int x_pos = 15;
    int y_pos = 5;

    while (1) {
        gotoXY(0, 0);

        if (GetKey('q')) {
            free_buffer(buffer, height);
            return 0;
        }

        reset_buffer(buffer, height, width);

        if (GetKey('w')) {
            y_pos--;
            if (y_pos < 0) {
                y_pos = height - 1;
            }
        }

        if (GetKey('s')) {
            y_pos++;
            if (y_pos > height - 1) {
                y_pos = 0;
            }
        }

        if (GetKey('a')) {
            x_pos--;
            if (x_pos < 0) {
                x_pos = width - 1;
            }
        }

        if (GetKey('d')) {
            x_pos++;
            if (x_pos > width - 1) {
                x_pos = 0;
            }
        }
        buffer[y_pos][x_pos] = '#';

        for (int i = 0; i < height; i++) {
            printf("%s", buffer[i]);
        }
    }

    free_buffer(buffer, height);
    return 0;
}

void gotoXY(int column, int row) {
    COORD coord;
    coord.X = column;
    coord.Y = row;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!SetConsoleCursorPosition(hConsole, coord)) {
        system("cls");
        printf("%d", GetLastError());
    }
}

void reset_buffer(char** buffer, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            buffer[i][j] = ' ';
        }
        buffer[i][width] = '\n';
        buffer[i][width + 1] = '\0';
    }
}

void hide_cursor(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;

    info.dwSize = 100;
    info.bVisible = FALSE;

    SetConsoleCursorInfo(hConsole, &info);
}

bool GetKey(char input) {
    switch (input) {
        case 'w':
            return GetAsyncKeyState(0x57) & 0x0001;
            break;
        case 'a':
            return GetAsyncKeyState(0x41) & 0x0001;
            break;
        case 's':
            return GetAsyncKeyState(0x53) & 0x0001;
            break;
        case 'd':
            return GetAsyncKeyState(0x44) & 0x0001;
        case 'q':
            return GetAsyncKeyState(0x51) & 0x0001;
            break;
        default:
            return false;
    }
}

char** init_buffer(int height, int width) {
    char** buffer = (char**)malloc(height * sizeof(char*));

    for (int i = 0; i < height; i++) {
        buffer[i] = (char*)malloc((width + 2) * sizeof(char));
    }

    return buffer;
}

void free_buffer(char** buffer, int height) {
    for (int i = 0; i < height; i++) {
        free(buffer[i]);
    }
    free(buffer);
}
