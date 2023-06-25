#include <conio.h>  // kbhit()
#include <stdio.h>  // printf()
#include <stdlib.h> // system()
#include <string.h>

#include "../header/Screen.h"

#define SUDOKUDIGIT ".123456789"

/* Function */
void PrintGrid(int *sudoku_grid);
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

    int sudoku_grid[81];
    char buf[100];

    printf("Paste an 81 digit string: ");
    fgets(buf, 100, stdin);

    int len = strlen(buf) - 1;
    if (len != 81)
        printf("Not enough digit. There are %d digits\n", len);
    else {
        for (int i = 0; i < 81; i++)
            sudoku_grid[i] = buf[i] - 48;

        PrintGrid(sudoku_grid);
    }

    system("pause");
}

void GenerateSudoku(void)
{
    printf("Generate Sudoku\n");
    system("pause");
}

void PrintRow(int *sudoku_grid, int offset)
{
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) printf("|");
        printf("%c", SUDOKUDIGIT[sudoku_grid[i + offset]]);
        if (i % 3 != 2) printf(" ");
    }
    printf("|\n");
}

void PrintGrid(int *sudoku_grid)
{
    printf(".-----.-----.-----.\n");
    for (int i = 0; i < 9; i++) {
        PrintRow(sudoku_grid, i * 9);
        if (i % 3 == 2 && i != 8) printf(":----- ----- -----:\n");
    }
    printf("'-----'-----'-----'\n");
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