#include <conio.h>  // kbhit()
#include <stdio.h>  // printf()
#include <stdlib.h> // system()

#include "../header/Random.h"
#include "../header/Screen.h"

#define SUDOKUDIGIT ".123456789"

typedef struct {
    int idx;
    int row;
    int col;
    int box;
    int *possible;
    int dim;
} SolverCell;

/* Function */
int *ConvertGrid(char *sudokustring);
int SolveBackTrack_Wrapper(int *grid);
void BackTrackSolver();
int IsValidSudokuString(char *sudoku_string);
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
        case 1: // Paste a 81 digit string and print the sudoku
            PasteAndPrint();
            break;

        case 2: // Generate a random 81 digit string and print the sudoku
            GenerateSudoku();
            break;

        case 3: // Solve a given sudoku
            BackTrackSolver();
            system("pause");
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
    printf("\t3- Solve;\n");
    printf("\t4- Exit\n");
}

int *ConvertGrid(char *sudokustring)
{
    if (!IsValidSudokuString(sudokustring)) return NULL;

    int *sudokugrid = malloc(81 * sizeof(int));
    for (int i = 0; i < 81; i++)
        sudokugrid[i] = sudokustring[i] - 48;

    return sudokugrid;
}

int *PasteAndConvert(void)
{
    int *sudoku_grid;
    char buf[100];

    printf("Paste an 81 digit string: ");
    fgets(buf, 100, stdin);

    sudoku_grid = ConvertGrid(buf);

    return sudoku_grid;
}

void PasteAndPrint(void)
{
    printf("Paste And Print\n");

    int *sudoku_grid = PasteAndConvert();

    if (sudoku_grid == NULL)
        printf("Not a valid string\n");
    else
        PrintGrid(sudoku_grid);

    system("pause");
}

void GenerateSudoku(void)
{
    printf("Generate Sudoku\n");

    int sudoku_grid[81];

    for (int i = 0; i < 81; i++)
        sudoku_grid[i] = RandomIntFrom0ToMax(10);

    PrintGrid(sudoku_grid);

    system("pause");
}

void PrintSolverCell(SolverCell cell)
{
    printf("Position %d; Dim: %d\n", cell.idx, cell.dim);
    printf("Possibilities: ");
    for (int i = 0; i < cell.dim; i++)
        printf("%d ", cell.possible[i]);
    printf("|");
    for (int i = cell.dim; i < 9; i++)
        printf("%d ", cell.possible[i]);
    printf("\n");
}

int CheckNextNumber(SolverCell *cell, int *grid)
{
    if (cell->dim == 0) return 0;

    int number = cell->possible[cell->dim - 1], found = 0;

    for (int i = 0; i < 3 && !found; i++) {
        for (int j = 0; j < 3 && !found; j++) {
            int val = i * 3 + j;

            int row = cell->row + val;
            int col = cell->col + 9 * val;
            int box = cell->box + j + 9 * i;

            int valid_row = row != cell->idx && number == grid[row],
                valid_col = col != cell->idx && number == grid[col],
                valid_box = box != cell->idx && number == grid[box];

            if (valid_row || valid_col || valid_box) found = 1;
        }
    }

    return found ? 0 : number;
}

int SolveBackTrack_Ric(int *grid, SolverCell *all_cell, int dimCell, int cell)
{
    all_cell[cell].dim = 9;
    if (cell == dimCell) {
        // Solved
        return 1;
    }

    for (int i = 8; i >= 0; i--) {
        int number = CheckNextNumber(&all_cell[cell], grid);

        if (number == 0) {
            all_cell[cell].dim--;
            continue;
        }

        grid[all_cell[cell].idx] = number;

        if (SolveBackTrack_Ric(grid, all_cell, dimCell, cell + 1)) return 1;

        grid[all_cell[cell].idx] = 0;
        all_cell[cell].dim--;
    }

    return 0;
}

int SolveBackTrack_Wrapper(int *grid)
{
    // Count how many number remain
    int to_solve = 0;

    for (int i = 0; i < 81; i++)
        if (grid[i] == 0) to_solve++;
    SolverCell *all_cell = malloc(to_solve * sizeof(*all_cell));

    for (int i = 0, j = 0; i < 81; i++) {
        if (grid[i] != 0) continue;

        all_cell[j].idx = i;
        all_cell[j].row = i - i % 9;
        all_cell[j].col = i % 9;
        all_cell[j].box = all_cell[j].row - 9 * ((all_cell[j].row / 9) % 3) + 3 * (all_cell[j].col / 3);
        all_cell[j].dim = 9;
        all_cell[j].possible = calloc(9, sizeof(int));
        for (int k = 0; k < 9; k++)
            all_cell[j].possible[k] = 9 - k;

        j++;
    }

    if (SolveBackTrack_Ric(grid, all_cell, to_solve, 0)) {
        PrintGrid(grid);
        return 1;
    } else
        return 0;
}

void BackTrackSolver(void)
{
    printf("BackTrack\n");

    int *sudoku_grid = PasteAndConvert();

    if (sudoku_grid == NULL) {
        printf("Not a valid string\n");
        return;
    } else
        PrintGrid(sudoku_grid);

    if (!SolveBackTrack_Wrapper(sudoku_grid)) printf("Cannot be solved!\n");
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
    printf("\n.-----.-----.-----.\n");
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

// Takes a int value and choose the associates operation
int ChooseOperation(int value)
{
    switch (value) {
    case 49: // char '1'
        return 1;

    case 50: // char '2'
        return 2;

    case 51: // char '3'
        return 3;

    default: return -1;
    }
}