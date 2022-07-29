#include "algorithms\SA.h"
#include <algorithm>

SA::SA()
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

SA::SA(std::vector<std::vector<int>> sudoku, bool is_fixed[9][9])
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

std::vector<std::vector<int>> SA::Solve()
{
	// Initialize sudoku with a solution that only satisfy 3rd condition: 3x3 cube
	initState();
	return this->sudoku;
}

void SA::initState()
{
	int start_row, start_col;
	for (int cube = 0; cube < 9; cube++) {
		switch (cube)
		{
		case 0: start_row = 0; start_col = 0; break;
		case 1: start_row = 0; start_col = 3; break;
		case 2: start_row = 0; start_col = 6; break;
		case 3: start_row = 3; start_col = 0; break;
		case 4: start_row = 3; start_col = 3; break;
		case 5: start_row = 3; start_col = 6; break;
		case 6: start_row = 6; start_col = 0; break;
		case 7: start_row = 6; start_col = 3; break;
		default: 
			start_row = 6; start_col = 6; break;
		}
		solveCube(start_row, start_col);
	}
}

void SA::solveCube(int start_row, int start_col)
{
	std::vector<int> candidate;
	for (int c = 1; c <= 9; c++) {
		candidate.push_back(c);
	}

	for (int row = start_row; row < start_row + 3; row++) {
		for (int col = start_col; col < start_col + 3; col++)
		{
			if (this->sudoku[row][col] != 0) {
				auto pos = std::find(candidate.begin(), candidate.end(), this->sudoku[row][col]);
				candidate.erase(pos);
			}
		}
	}

	for (int row = start_row; row < start_row + 3; row++) {
		for (int col = start_col; col < start_col + 3; col++)
		{
			if (!this->is_fixed[row][col]) {
				this->sudoku[row][col] = candidate[0];
				candidate.erase(candidate.begin());
			}
		}
	}
}


