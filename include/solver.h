#include <iostream>
#include <fstream>
#include <chrono>
#include <stdlib.h>
using namespace std;

class Solver {
public:
    static const int MAX_DIM = 9;
    const int SUM = (MAX_DIM+1)*MAX_DIM/2;
    int sudoku[MAX_DIM][MAX_DIM];
    int result_backtracking[MAX_DIM][MAX_DIM];
    int result_sa[MAX_DIM][MAX_DIM];
    bool is_fixed[MAX_DIM][MAX_DIM];
    int current_cost;
    // Read sudoku from input.txt
    void get_input() {
        ifstream Input("./src/input.txt");
        string temp;
        int row = 0;
        int col = 0;
        while (getline(Input, temp)) {  
            while (temp.length() != 0) {
                sudoku[row][col] = temp[0] - '0';
                result_backtracking[row][col] = sudoku[row][col];
                result_sa[row][col] = sudoku[row][col];
                is_fixed[row][col] = (sudoku[row][col] == 0) ? false : true;
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
    // Find next unassigned cell and save row and col
    bool next_unassigned_cell(int sudoku[MAX_DIM][MAX_DIM], int& row, int& col) {
        for (row = 0; row < MAX_DIM; row++) {
            for (col = 0; col < MAX_DIM; col++) {
                if (sudoku[row][col] == 0) 
                    return true;
            }
        }
        return false;
    }

    // Check if a candidate is safed to put in the cell
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

    // Backtracking driver: 
    // 1. Try each candidate from 1-9
    // 2. If ok then assign and bactracking(the new sudoku)
    // 3. If all cells are assigned the return true
    // 4. If not then re-assigned previous solved cell with 0 to backtrack
    // 5. Return false if no solution is found
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

    double solve_backtracking()
    {
        // Reset result at beginning of each algo
        for (int row = 0; row < MAX_DIM; row++) {
            for (int col = 0; col < MAX_DIM; col++) {
                result_backtracking[row][col] = sudoku[row][col];
            }
        }
        auto start = chrono::high_resolution_clock::now();
        backtracking(result_backtracking);
        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = stop - start;
        return duration.count();
    }
    #pragma endregion Backtracking

    /*--------------------------------------Simulated Annealing-------------------------------------*/
    #pragma region Simulated Annealing

    void sa_init_state() {
        for (int row = 0; row < MAX_DIM; row++) {
            for (int col = 0; col < MAX_DIM; col++) {
                if (!is_fixed[row][col]) {
                    for (int c = 1; c <= 9; c++) {
                        bool existed = false;
                        for (int j = 0; j < 9; j++) {
                            if (c == result_sa[row][j]) {
                                existed = true;
                                break;
                            }
                        }
                        if (!existed) {
                            result_sa[row][col] = c; 
                            break;
                        }
                    }
                }
            }
        }
    }

    int cost(int grid[MAX_DIM][MAX_DIM]) {
        int cost = 0;
        for (int row = 0; row < MAX_DIM-1; row++) {
            for (int col = 0; col < MAX_DIM-1; col++) {
                for (int i = row+1; i < MAX_DIM; i++) {
                    if (grid[row][col] == grid[i][col]) cost++;
                }
                for (int i = col+1; i < MAX_DIM; i++) {
                    if (grid[row][col] == grid[row][i]) cost++;
                }
            }
        }
        return cost;
    }

    int switch_cells(int start_row, int start_col, int current_cost) {
        
    }

    void sa(int[MAX_DIM][MAX_DIM]) {
        sa_init_state();
        current_cost = cost(result_sa);
        // while (current_cost > 0) {
        //     // Choose random 3x3 grid
        //     int grid = rand() % 9 + 1;
        //     int start_row, start_col;
        //     switch (grid)
        //     {
        //     case 2: start_row = 0; start_col = 3; break;
        //     case 3: start_row = 0; start_col = 6; break;
        //     case 4: start_row = 3; start_col = 0; break;
        //     case 5: start_row = 3; start_col = 3; break;
        //     case 6: start_row = 3; start_col = 6; break;
        //     case 7: start_row = 6; start_col = 0; break;
        //     case 8: start_row = 6; start_col = 3; break;
        //     case 9: start_row = 6; start_col = 6; break;
        //     default:
        //         start_row = 0; start_col = 0; break;
        //         break;
        //     }
        //     switch_cells(start_row, start_col, current_cost);
        //     cout << current_cost;
        // }
    }

    double solve_sa() {
        auto start = chrono::high_resolution_clock::now();
        sa(result_sa);
        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = stop - start;
        return duration.count();
    }

    #pragma endregion Simulated Annealing
    
};