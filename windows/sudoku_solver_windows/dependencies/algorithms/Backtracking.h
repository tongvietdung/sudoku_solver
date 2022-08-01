#ifndef BACKTRACKING_H
#define BACKTRACKING_H
#include <vector>
#include "imgui.h"

class Backtracking
{
public:
	Backtracking();
	Backtracking(std::vector<std::vector<int>>, bool[9][9]);
	std::vector<std::vector<int>> Solve();
private:
	std::vector<std::vector<int>> sudoku;
	bool is_fixed[9][9];
	bool next_unassigned_cell(std::vector<std::vector<int>>, int&, int&);
	bool is_valid(std::vector<std::vector<int>>, int, int, int);
	bool backtrack(std::vector<std::vector<int>>, bool[9][9]);
	void print_sudoku();
};
#endif