#ifndef SA_H
#define SA_H
#include <vector>

#include "imgui.h"
class SA
{
public:
	SA();
	SA(std::vector<std::vector<int>>, bool[9][9]);
	std::vector<std::vector<int>> Solve();
private:
	std::vector<std::vector<int>> sudoku;
	bool is_fixed[9][9];
	void initState();
	void solveSquare(int, int);
	int cost();
	int randomState(float);
};
#endif