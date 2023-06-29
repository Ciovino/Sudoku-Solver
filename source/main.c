#include <conio.h>  // kbhit()
#include <stdio.h>  // printf()
#include <stdlib.h> // system()

#include "../header/Colori.h"
#include "../header/Random.h"
#include "../header/Screen.h"

#define SUDOKUDIGIT ".123456789"
#define UP          119
#define DOWN        115
#define RIGHT       100
#define LEFT        97

typedef enum { FIXED = -1, EMPTY, CORRECT, WRONG } SudokuState;

typedef struct {
    int value;
    SudokuState state;
} SudokuCell;

/* Function */
int IsValidSudokuString(char *sudoku_string);
void PrintGrid(SudokuCell *sudoku_grid);
void PasteAndPlay(void);
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

            case 2: PasteAndPlay(); break;

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

SudokuCell NewSudokuCell(int number)
{
    SudokuCell cell;

    cell.value = number;
    if (number == 0)
        cell.state = EMPTY;
    else
        cell.state = FIXED;

    return cell;
}

void PasteAndPrint(void)
{
    ClearAndHome();
    printf("Paste And Print\n");

    SudokuCell sudoku_grid[81];
    char buf[100];

    printf("Paste an 81 digit string: ");
    fgets(buf, 100, stdin);

    if (!IsValidSudokuString(buf))
        printf("Not a valid string\n");
    else {
        for (int i = 0; i < 81; i++)
            sudoku_grid[i] = NewSudokuCell(buf[i] - 48);

        PrintGrid(sudoku_grid);
    }

    system("pause");
}

int CompleteSudoku(SudokuCell *grid)
{
    for (int i = 0; i < 81; i++)
        if (grid[i].value == 0) return 0;
    return 1;
}

void PrintPlayGrid(SudokuCell *grid, int pos)
{
    ClearAndHome();
    printf(".-----.-----.-----.\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0) printf("|");

            if (j + i * 9 == pos)
                BackGroundAndText(COL_WHITE, COL_BLACK);
            else {
                switch (grid[j + i * 9].state) {
                case CORRECT: Textcolor(COL_LIGHT_GREEN); break;
                case WRONG: Textcolor(COL_RED); break;

                case FIXED:
                case EMPTY:
                default: Textcolor(COL_NORMAL); break;
                }
            }

            printf("%c", SUDOKUDIGIT[grid[j + i * 9].value]);
            Textcolor(COL_NORMAL);

            if (j % 3 != 2) printf(" ");
        }
        printf("|\n");
        if (i % 3 == 2 && i != 8) printf(":----- ----- -----:\n");
    }
    printf("'-----'-----'-----'\n");
}

SudokuCell ChangeSudokuCell(SudokuCell *grid, int pos, int new_number)
{
    SudokuCell previous = grid[pos];
    if (previous.state == FIXED || previous.value == new_number) return previous;

    SudokuCell new;
    new.value = new_number;

    if (new_number == 0) {
        new.state = EMPTY;
        return new;
    }

    int row = pos - pos % 9, col = pos % 9, box = row - 9 * ((row / 9) % 3) + 3 * (col / 3), found = 0;
    for (int i = 0; i < 3 && !found; i++) {
        for (int j = 0; j < 3 && !found; j++) {
            int val = i * 3 + j;

            int next_row = row + val;
            int next_col = col + 9 * val;
            int next_box = box + j + 9 * i;

            int valid_row = next_row != pos && new_number == grid[next_row].value,
                valid_col = next_col != pos && new_number == grid[next_col].value,
                valid_box = next_box != pos && new_number == grid[next_box].value;

            if (valid_row || valid_col || valid_box) found = 1;
        }
    }

    new.state = found ? WRONG : CORRECT;

    return new;
}

void Play(SudokuCell *grid)
{
    int pos = 0;
    PrintPlayGrid(grid, pos);

    while (!CompleteSudoku(grid)) {
        int key;
        while (!kbhit()) {}

        key = getch();

        switch (key) {
        case UP:
            pos -= 9;
            if (pos < 0) pos += 81;
            break;

        case DOWN:
            pos += 9;
            if (pos >= 81) pos -= 81;
            break;

        case RIGHT:
            pos++;
            if (pos >= 81) pos = 0;
            break;

        case LEFT:
            pos--;
            if (pos < 0) pos = 80;
            break;

        case 48: // 0 -> Delete cell value
        case 49:
        case 50:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57: grid[pos] = ChangeSudokuCell(grid, pos, key - 48); break;

        default: break;
        }

        PrintPlayGrid(grid, pos);
    }
}

void PasteAndPlay(void)
{
    ClearAndHome();
    printf("Paste And Play\n");

    SudokuCell sudoku_grid[81];
    char buf[100];

    printf("Paste an 81 digit string: ");
    fgets(buf, 100, stdin);

    if (!IsValidSudokuString(buf))
        printf("Not a valid string\n");
    else {
        for (int i = 0; i < 81; i++)
            sudoku_grid[i] = NewSudokuCell(buf[i] - 48);

        PrintGrid(sudoku_grid);
    }

    Play(sudoku_grid);

    system("pause");
}

void GenerateSudoku(void)
{
    ClearAndHome();
    printf("Generate Sudoku\n");

    SudokuCell sudoku_grid[81];

    for (int i = 0; i < 81; i++)
        sudoku_grid[i] = NewSudokuCell(RandomIntFrom0ToMax(10));

    PrintGrid(sudoku_grid);

    system("pause");
}

void PrintRow(SudokuCell *sudoku_grid, int offset)
{
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) printf("|");
        printf("%c", SUDOKUDIGIT[sudoku_grid[i + offset].value]);
        if (i % 3 != 2) printf(" ");
    }
    printf("|\n");
}

void PrintGrid(SudokuCell *sudoku_grid)
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