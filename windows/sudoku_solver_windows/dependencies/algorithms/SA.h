#ifndef SA_H
#define SA_H
#include <vector>

class SA
{
public:
	SA();
	SA(std::vector<std::vector<int>> sudoku);
	std::vector<std::vector<int>> Solve();
private:
	std::vector<std::vector<int>> sudoku;

};
#endif