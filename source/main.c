#include <conio.h>  // kbhit()
#include <stdio.h>  // printf()
#include <stdlib.h> // system()

#include "../header/Screen.h"

/* Function */
int ChooseOperation(int value);

int main(int argc, char **argv)
{
    ClearAndHome();
    printf("Simple Sudoku Solver\n");

    int menu_value = -1;
    int keep_going = 1;
    do {
        menu_value = getch();
        menu_value = ChooseOperation(menu_value);

        switch (menu_value) {
        case -1: keep_going = 0; break;
        default: printf("You press: %d\n", menu_value); break;
        }

    } while (keep_going);

    return 0;
}

int ChooseOperation(int value)
{
    // From Ascii table: char '0' => int 48; char '9' => int 57
    if (value >= 48 && value <= 57) return (value - 48);

    return -1;
}