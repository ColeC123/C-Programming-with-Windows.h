#ifndef CONSOLEFUNCTIONS_H
#define CONSOLEFUNCTIONS_H
#include "windows.h"
#include "stdio.h"
#include "stdbool.h"

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

void hide_cursor(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;

    info.dwSize = 100;
    info.bVisible = FALSE;

    SetConsoleCursorInfo(hConsole, &info);
}

void show_cursor(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;

    info.dwSize = 100;
    info.bVisible = TRUE;

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

#endif