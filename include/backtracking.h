#include <iostream>
#include <fstream>
#include <chrono>
using namespace std;

class Backtracking {
public:
    static const int MAX_DIM = 9;
    const int SUM = (MAX_DIM+1)*MAX_DIM/2;
    int sudoku[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    // Read sudoku from input.txt
    void get_input() {
        ifstream Input("./src/input.txt");
        string temp;
        int row = 0;
        int col = 0;
        while (getline(Input, temp)) {  
            while (temp.length() != 0) {
                sudoku[row][col] = temp[0] - '0';
                result[row][col] = sudoku[row][col];
                temp.erase(0, 2);
                col++;
            }
            row++;
            col = 0;
        }
        // Close input
        Input.close();
    }

    /*-----------------------------------------Backtracking-----------------------------------------*/
    #pragma region Backtracking
    bool next_unassigned_cell(int sudoku[MAX_DIM][MAX_DIM], int& row, int& col) {
        for (row = 0; row < MAX_DIM; row++) {
            for (col = 0; col < MAX_DIM; col++) {
                if (sudoku[row][col] == 0) 
                    return true;
            }
        }
        return false;
    }

    bool is_valid(int sudoku[MAX_DIM][MAX_DIM], int row, int col, int candidate) {
        // Check empty cell
        if (sudoku[row][col] != 0) return false;
        // Check row and column
        for (int i = 0; i < MAX_DIM; i++) {
            if (sudoku[row][i] == candidate || candidate == sudoku[i][col]) {
                return false;
            }
        }

        // Check 3x3 grid
        for (int i = 0; i < 3 ; i++ ) {
            for (int j = 0; j < 3 ; j++) {
                if (candidate == sudoku[i + row - row % 3][j + col - col % 3]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool backtracking(int sudoku[MAX_DIM][MAX_DIM]) {
        int row, col;

        // Complete the sudoku with no unassigned cell
        if (!next_unassigned_cell(sudoku, row, col)) {
            return true;
        }

        // Test possible candidates from 1-9
        for (int candidate = 1; candidate <= MAX_DIM; candidate++) {
            if (is_valid(sudoku, row, col, candidate)) {
                // If candidate is valid, assigned to cell
                sudoku[row][col] = candidate;

                // Recursive find next solution from current one
                if (backtracking(sudoku))
                    return true;
                
                sudoku[row][col] = 0;
            }
        }
        return false;
    }
    #pragma endregion Backtracking

    /*--------------------------------------Simulated Annealing-------------------------------------*/
    #pragma region Simulated Annealing
    #pragma endregion Simulated Annealing

    /*---------------------------------------------Main---------------------------------------------*/
    double solve()
    {
        auto start = chrono::high_resolution_clock::now();
        backtracking(result);
        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = stop - start;
        return duration.count();
    }
};