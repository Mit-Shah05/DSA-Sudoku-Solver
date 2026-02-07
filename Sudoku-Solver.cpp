#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

int sudokuSize;
int temp;

void displayGrid(const vector<vector<int>> &grid)
{
    int width = log10(sudokuSize) + 2;
    int totalWidth = (width + 1) * sudokuSize + temp + 1;

    for (int i = 0; i < sudokuSize; i++)
    {
        if (i % temp == 0)
        {
            for (int k = 0; k < totalWidth; k++)
                cout << "-";
            cout << "\n";
        }

        for (int j = 0; j < sudokuSize; j++)
        {
            if (j % temp == 0)
                cout << "|";

            cout << setw(width) << grid[i][j] << " ";
        }
        cout << "|\n";
    }

    for (int k = 0; k < totalWidth; k++)
        cout << "-";
    cout << "\n";
}

bool isValidGrid(const vector<vector<int>> &grid)
{
    vector<vector<int>> rowCheck(sudokuSize, vector<int>(sudokuSize, 0));
    vector<vector<int>> colCheck(sudokuSize, vector<int>(sudokuSize, 0));
    vector<vector<int>> subgridCheck(sudokuSize, vector<int>(sudokuSize, 0));

    for (int i = 0; i < sudokuSize; i++)
    {
        for (int j = 0; j < sudokuSize; j++)
        {
            int num = grid[i][j];
            if (num != 0)
            {
                int subgridIndex = (i / temp) * temp + (j / temp);

                if (rowCheck[i][num - 1] ||
                    colCheck[j][num - 1] ||
                    subgridCheck[subgridIndex][num - 1])
                {
                    return false;
                }

                rowCheck[i][num - 1] = 1;
                colCheck[j][num - 1] = 1;
                subgridCheck[subgridIndex][num - 1] = 1;
            }
        }
    }
    return true;
}

string estimateDifficulty(const vector<vector<int>> &grid)
{
    int filledCells = 0;

    for (const auto &row : grid)
        filledCells += count_if(row.begin(), row.end(),
                                [](int num)
                                { return num != 0; });

    double fillRatio = (double)filledCells / (sudokuSize * sudokuSize);

    if (fillRatio > 0.5)
        return "Easy";
    else if (fillRatio > 0.3)
        return "Medium";
    else
        return "Hard";
}

bool findEmptyCell(vector<vector<int>> &arr, int &row, int &col)
{
    for (int i = 0; i < sudokuSize; i++)
    {
        for (int j = 0; j < sudokuSize; j++)
        {
            if (arr[i][j] == 0)
            {
                row = i;
                col = j;
                return true;
            }
        }
    }
    return false;
}

bool presentInSubgrid(int row, int col, int number, vector<vector<int>> &arr)
{
    int startRow = (row / temp) * temp;
    int startCol = (col / temp) * temp;

    for (int i = 0; i < temp; i++)
    {
        for (int j = 0; j < temp; j++)
        {
            if (arr[startRow + i][startCol + j] == number)
                return true;
        }
    }
    return false;
}

bool sudokuSolver(vector<vector<int>> &arr,
                  vector<vector<int>> &rowBacktracking,
                  vector<vector<int>> &colBacktracking)
{
    int row, col;

    if (!findEmptyCell(arr, row, col))
        return true;

    for (int num = 1; num <= sudokuSize; num++)
    {
        if (!rowBacktracking[row][num - 1] &&
            !colBacktracking[col][num - 1] &&
            !presentInSubgrid(row, col, num, arr))
        {
            arr[row][col] = num;
            rowBacktracking[row][num - 1] = 1;
            colBacktracking[col][num - 1] = 1;

            if (sudokuSolver(arr, rowBacktracking, colBacktracking))
                return true;

            arr[row][col] = 0;
            rowBacktracking[row][num - 1] = 0;
            colBacktracking[col][num - 1] = 0;
        }
    }
    return false;
}

bool letsSolveSudoku(vector<vector<int>> &arr)
{
    vector<vector<int>> rowBacktracking(sudokuSize, vector<int>(sudokuSize, 0));
    vector<vector<int>> colBacktracking(sudokuSize, vector<int>(sudokuSize, 0));

    for (int i = 0; i < sudokuSize; i++)
    {
        for (int j = 0; j < sudokuSize; j++)
        {
            int num = arr[i][j];
            if (num != 0)
            {
                rowBacktracking[i][num - 1] = 1;
                colBacktracking[j][num - 1] = 1;
            }
        }
    }

    return sudokuSolver(arr, rowBacktracking, colBacktracking);
}

int main()
{
    cout << "\n!!! Welcome to Sudoku Solver Programme !!!\n";

    while (true)
    {
        cout << "\nEnter size of Sudoku (e.g., 9) or -1 to exit: ";
        cin >> sudokuSize;

        if (sudokuSize == -1)
            break;

        if (sudokuSize <= 0)
        {
            cout << "Invalid size. Try again.\n";
            continue;
        }

        temp = sqrt(sudokuSize);

        vector<vector<int>> grid(sudokuSize, vector<int>(sudokuSize));

        cout << "Enter Sudoku Grid (use 0 for empty cells):\n";
        for (int i = 0; i < sudokuSize; i++)
        {
            for (int j = 0; j < sudokuSize; j++)
                cin >> grid[i][j];
        }

        if (!isValidGrid(grid))
        {
            cout << "Invalid Sudoku grid: duplicate values found.\n";
            continue;
        }

        cout << "Estimated Difficulty: " << estimateDifficulty(grid) << "\n";

        if (letsSolveSudoku(grid))
        {
            cout << "\nBingo!!! Solution:\n\n";
            displayGrid(grid);
        }
        else
        {
            cout << "No solution exists for the given Sudoku.\n";
        }

        char choice;
        cout << "\nDo you want to continue? (y/n): ";
        cin >> choice;

        if (choice == 'n' || choice == 'N')
            break;
    }

    return 0;
}
