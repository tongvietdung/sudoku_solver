
#include <algorithm>
#include <iostream>
using namespace std;

class SA
{

private:
    vector<vector<int>> solve_grid(vector<vector<int>> sudoku, vector<vector<bool>> is_fixed, int start_row, int start_col) {
        vector<vector<int>> result = sudoku;
        vector<int> used_num;
        for (int row = start_row; row < start_row + 3; row++) {
            for (int col = start_col; col < start_col + 3; col++) {
                if (find(used_num.begin(), used_num.end(), result.at(row).at(col)) == used_num.end()) {
                    used_num.push_back(result.at(row).at(col));
                }
            }
        }
        for (int row = start_row; row < start_row + 3; row++) {
            for (int col = start_col; col < start_col + 3; col++) {
                if (!is_fixed.at(row).at(col)) {
                    for (int c = 1; c <= 9; c++) {
                        if (find(used_num.begin(), used_num.end(), c) == used_num.end()) {
                            result.at(row).at(col) = c;
                            used_num.push_back(c);
                            break;
                        }
                    }   
                }
            }
        }
        return result;
    }

    vector<vector<int>> init_state(vector<vector<int>> sudoku, vector<vector<bool>> is_fixed) {
        vector<vector<int>> result = sudoku;
        for (int grid = 0; grid < 9; grid++) {
            int start_row, start_col;
            switch (grid)
            {
            case 1: start_row = 0; start_col = 3; break;
            case 2: start_row = 0; start_col = 6; break;
            case 3: start_row = 3; start_col = 0; break;
            case 4: start_row = 3; start_col = 3; break;
            case 5: start_row = 3; start_col = 6; break;
            case 6: start_row = 6; start_col = 0; break;
            case 7: start_row = 6; start_col = 3; break;
            case 8: start_row = 6; start_col = 6; break;
            default: start_row = 0; start_col = 0;break;
            }
            result = solve_grid(result, is_fixed, start_row, start_col);
        }
        return result;
    }

    int cost(vector<vector<int>> sudoku) {
        int result = 0;
        // Cost in row
        for (auto row : sudoku) {
            auto temp = row;
            sort(temp.begin(), temp.end());
            auto new_end = unique(temp.begin(), temp.end());
            temp.resize(distance(temp.begin(), new_end));
            result += 9-temp.size();
        }
        // Cost in column
        for (int col = 0; col < 9; col++) {
            vector<int> temp;
            for (int row = 0; row < 9; row++) {
                temp.push_back(sudoku.at(row).at(col));
            }
            sort(temp.begin(), temp.end());
            auto new_end = unique(temp.begin(), temp.end());
            temp.resize(distance(temp.begin(), new_end));
            result += 9-temp.size();
        }
        return result;
    }

public:
    vector<vector<int>> solve(vector<vector<int>> sudoku, vector<vector<bool>> is_fixed) {
        vector<vector<int>> result = sudoku;
        result = init_state(result, is_fixed);
        cout << cost(result) << endl;
        return result;
    }

};



