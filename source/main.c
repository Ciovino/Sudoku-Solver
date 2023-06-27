#include <conio.h>  // kbhit()
#include <stdio.h>  // printf()
#include <stdlib.h> // system()

#include "../header/Random.h"
#include "../header/Screen.h"

#define SUDOKUDIGIT ".123456789"

/* Function */
int IsValidSudokuString(char *sudoku_string);
void PrintGrid(int *sudoku_grid);
void PasteAndPrint(void);
void GenerateSudoku(void);
void PrintMenu(void);
void PrintSubMenu(int submenu);
int ChooseOperation(int value);
int ChooseOperation_SubMenu(int menu, int value);

int main(int argc, char **argv)
{
    int menu_value = -1;
    int submenu_value = -1;
    int keep_going = 1;
    do {
        PrintMenu();

        menu_value = getch();
        menu_value = ChooseOperation(menu_value);

        switch (menu_value) {
        case 1: // Paste a 81 digit string and print the sudoku
            // Sub menu
            PrintSubMenu(menu_value);
            submenu_value = getch();
            submenu_value = ChooseOperation_SubMenu(menu_value, submenu_value);

            switch (submenu_value) {
            case 1: PasteAndPrint(); break;

            case 2:
                printf("Paste and Play\n");
                system("pause");
                break;

            case -1:
            default: break;
            }

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

void PasteAndPrint(void)
{
    ClearAndHome();
    printf("Paste And Print\n");

    int sudoku_grid[81];
    char buf[100];

    printf("Paste an 81 digit string: ");
    fgets(buf, 100, stdin);

    if (!IsValidSudokuString(buf))
        printf("Not a valid string\n");
    else {
        for (int i = 0; i < 81; i++)
            sudoku_grid[i] = buf[i] - 48;

        PrintGrid(sudoku_grid);
    }

    system("pause");
}

void GenerateSudoku(void)
{
    ClearAndHome();
    printf("Generate Sudoku\n");

    int sudoku_grid[81];

    for (int i = 0; i < 81; i++)
        sudoku_grid[i] = RandomIntFrom0ToMax(10);

    PrintGrid(sudoku_grid);

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

int isDigit(char c)
{
    if (c >= 48 && c <= 57) return 1;
    return 0;
}

int IsValidSudokuString(char *sudoku_string)
{
    int isValid = 1;
    int i = 0;

    while (sudoku_string[i + 1] != '\0' && isValid) {
        isValid = isDigit(sudoku_string[i]);
        i++;
    }

    if (i != 81) isValid = 0;

    return isValid;
}

void PrintMenu(void)
{
    ClearAndHome();
    printf("Simple Sudoku Solver\n");

    printf("What do you want to do?\n");
    printf("\t1- Play;\n");
    printf("\t2- Generate Sudoku;\n");
    printf("\t3- Exit\n");
}

void PrintSubMenu(int submenu)
{
    ClearAndHome();

    switch (submenu) {
    case 1: // Play
        printf("Play!!\n");

        printf("\t1- Paste and Print;\n");
        printf("\t2- Paste and Play.\n");
        break;

    default: break;
    }
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

int ChooseOperation_SubMenu(int menu, int value)
{
    switch (menu) {
    case 1: // Play
        switch (value) {
        case 49: // char '1'
            return 1;

        case 50: // char '2'
            return 2;

        default: return -1;
        }

    default: return -1;
    }
}