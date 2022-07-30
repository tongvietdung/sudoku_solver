#include "algorithms\SA.h"
#include <algorithm>
#include <random>
#include <chrono>

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

void SA::initState()
{
	int start_row, start_col;
	// Iterate through all 9 3x3 squares and solve them
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
		solveSquare(start_row, start_col);
	}
}

void SA::solveSquare(int start_row, int start_col)
{
	// Generate a vector of candidates
	std::vector<int> candidate;
	for (int c = 1; c <= 9; c++) {
		candidate.push_back(c);
	}

	// Remove all candidates that already appeared in the cube
	for (int row = start_row; row < start_row + 3; row++) {
		for (int col = start_col; col < start_col + 3; col++)
		{
			if (this->sudoku[row][col] != 0) {
				auto pos = std::find(candidate.begin(), candidate.end(), this->sudoku[row][col]);
				candidate.erase(pos);
			}
		}
	}

	// Solve
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

// Cost function calculates the number of duplicates in row/column-wise.
int SA::cost() {
	int res = 0;
	std::vector<int> temp = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Calculate row-wise cost
	for (auto row : this->sudoku) {
		temp = row;
		std::sort(temp.begin(), temp.end());
		auto it = std::unique(temp.begin(), temp.end());
		temp.resize(std::distance(temp.begin(), it));
		res += 9 - temp.size();
	}

	// Calculate column-wise cost
	for (int col = 0; col < 9; col++) {
		std::vector<int> temp;
		for (int row = 0; row < 9; row++) {
			temp.push_back(this->sudoku[row][col]);
		}
		std::sort(temp.begin(), temp.end());
		auto it = std::unique(temp.begin(), temp.end());
		temp.resize(std::distance(temp.begin(), it));
		res += 9 - temp.size();
	}

	return res;
}

void SA::randomState() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> rand1(0, 8);

	int row1, col1, row2, col2, square_row_min, square_col_min;
	row1 = rand1(generator);
	col1 = rand1(generator);
	
	while (this->is_fixed[row1][col1]) {
		row1 = rand1(generator);
		col1 = rand1(generator);
	}
	
	square_row_min = row1 - row1 % 3;
	square_col_min = col1 - col1 % 3;
	std::uniform_int_distribution<int> rand_row(square_row_min, square_row_min + 2);
	std::uniform_int_distribution<int> rand_col(square_col_min, square_col_min + 2);
	row2 = rand_row(generator);
	col2 = rand_col(generator);


	while (this->is_fixed[row2][col2]) {
		if (!(row1 != row2 && col1 != col2)) {
			row2 = rand_row(generator);
			col2 = rand_col(generator);
		}	
	}

	// Swap
	int temp;
	temp = this->sudoku[row1][col1];
	this->sudoku[row1][col1] = this->sudoku[row2][col2];
	this->sudoku[row2][col2] = this->sudoku[row1][col1];
}

std::vector<std::vector<int>> SA::Solve()
{
	// Initialize sudoku with a solution that only satisfy 3rd condition: 3x3 cube
	initState();
	randomState();
	return this->sudoku;
}

