#include <conio.h>  // kbhit()
#include <stdio.h>  // printf()
#include <stdlib.h> // system()

#include "../header/Screen.h"

/* Function */
void PrintMenu(void);
void PasteAndPrint(void);
void GenerateSudoku(void);
int ChooseOperation(int value);

int main(int argc, char **argv)
{
    int menu_value = -1;
    int keep_going = 1;
    do {
        PrintMenu();

        menu_value = getch();
        menu_value = ChooseOperation(menu_value);

        switch (menu_value) {
        case 1: // Paste a 81 digit string e print the sudoku
            PasteAndPrint();
            break;

        case 2: // Generate a random 81 digit string and print the sudoku
            GenerateSudoku();
            break;

        case -1:
        default:
            printf("Bye Bye\n");
            keep_going = 0;
            break;
        }

    } while (keep_going);

    return 0;
}

void PrintMenu(void)
{
    ClearAndHome();
    printf("Simple Sudoku Solver\n");

    printf("What do you want to do?\n");
    printf("\t1- Paste and print;\n");
    printf("\t2- Generate Sudoku;\n");
    printf("\t3- Exit\n");
}

void PasteAndPrint(void)
{
    printf("Paste And Print\n");
    system("pause");
}

void GenerateSudoku(void)
{
    printf("Generate Sudoku\n");
    system("pause");
}

// Takes a int value and choose the associates operation
int ChooseOperation(int value)
{
    switch (value) {
    case 49: // char '1'
        return 1;

    case 50: // char '2'
        return 2;

    case 51: // char '3'
    default: return -1;
    }
}