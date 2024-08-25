#ifndef CONSOLEFUNCTIONS_H
#define CONSOLEFUNCTIONS_H
#include "windows.h"
#include "stdio.h"
#include "stdbool.h"

enum keys {
    wKey = 0x57,
    aKey = 0x41,
    sKey = 0x53,
    dKey = 0x44,
    qKey = 0x51,
    upArrow = 0x26,
    downArrow = 0x28,
    rightArrow = 0x27,
    leftArrow = 0x25,
};

//Sets the position of the cursor in the terminal
//This code is specific to windows, since it uses the win32 api (windows.h)
void gotoXY(int column, int row) {
    COORD coord;
    coord.X = column;
    coord.Y = row;

    //Gets the standard output handle (a handle is like a pointer to a system resource)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!SetConsoleCursorPosition(hConsole, coord)) {
        system("cls");
        printf("SetConsoleCursorPosition Failed");
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

//GetAsyncKeyState indicates whether the key has been pressed since the last query
/*
The use of bitwise and with 1 is used to get the least significant bit of the return value,
and return 1 if the least significant bit is 1. This is done because when the least significant bit
is equal to 1, that means that the key has been pressed since the last query made with the GetAsyncKeyState
function. There are some caveats though: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate#remarks
*/
bool GetKey(enum keys input) {
    return GetAsyncKeyState(input) & 0x0001;
}

#endif