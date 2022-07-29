#include "algorithms\SA.h"

SA::SA()
{
	for (int row = 0; row < 9; row++) {
		std::vector<int> col_vec;
		for (int col = 0; col < 9; col++) {
			col_vec.push_back(0);
		}
		this->sudoku.push_back(col_vec);
	}
}

SA::SA(std::vector<std::vector<int>> sudoku)
{
	for (auto row : sudoku) {
		this->sudoku.push_back(row);
	}
}

std::vector<std::vector<int>> SA::Solve()
{

	return this->sudoku;
}


