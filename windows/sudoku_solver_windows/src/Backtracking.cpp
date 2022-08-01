#include "algorithms\Backtracking.h"
#include <iostream>

Backtracking::Backtracking()
{
    for (int row = 0; row < 9; row++) {
        std::vector<int> col_vec;
        for (int col = 0; col < 9; col++) {
            col_vec.push_back(0);
            is_fixed[row][col] = false;
        }
        this->sudoku.push_back(col_vec);
    }
}

Backtracking::Backtracking(std::vector<std::vector<int>> sudoku, bool is_fixed[9][9])
{
    for (auto row : sudoku) {
        this->sudoku.push_back(row);
    }

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            this->is_fixed[row][col] = is_fixed[row][col];
        }
    }
}

std::vector<std::vector<int>> Backtracking::Solve()
{
	backtrack(this->sudoku, this->is_fixed);
	return this->sudoku;
}

bool Backtracking::next_unassigned_cell(std::vector<std::vector<int>> sudoku, int& row, int& col)
{
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (sudoku[row][col] == 0)
                return true;
        }
    }
	return false;
}

bool Backtracking::is_valid(std::vector<std::vector<int>> sudoku, int row, int col, int candidate)
{   // Check empty cell
    if (sudoku[row][col] != 0) return false;

    // Check row and column
    for (int i = 0; i < 9; i++) {
        if (sudoku[row][i] == candidate || candidate == sudoku[i][col]) {
            return false;
        }
    }

    // Check 3x3 square
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (candidate == sudoku[i + row - row % 3][j + col - col % 3]) {
                return false;
            }
        }
    }
    return true;
}

bool Backtracking::backtrack(std::vector<std::vector<int>> sudoku, bool is_fixed[9][9])
{
	int row, col;

    // Complete the sudoku with no unassigned cell
    if (!next_unassigned_cell(sudoku, row, col)) {
        return true;
    }

    // Test possible candidates from 1-9
    for (int candidate = 1; candidate <= 9; candidate++) {
        if (is_valid(sudoku, row, col, candidate)) {
            // If candidate is valid, assigned to cell
            sudoku[row][col] = candidate;
            this->sudoku = sudoku;
            print_sudoku();
            // Recursive find next solution from current one
            if (backtrack(sudoku, is_fixed)) {
                return true;
            }
            sudoku[row][col] = 0;
        }
    }
	return false;
}
void Backtracking::print_sudoku() {
    for (int row = 0; row < 9; row++) 
    {
        for (int col = 0; col < 9; col++)
        {
            std::cout << this->sudoku[row][col] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------" << std::endl;
}

