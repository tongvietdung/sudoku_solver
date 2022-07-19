#include <fstream>
#include <chrono>
using namespace std;

class SA {
public:
    static const int MAX_DIM = 9;
    const int SUM = (MAX_DIM+1)*MAX_DIM/2;
    int sudoku[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];
    bool is_fixed[MAX_DIM][MAX_DIM];

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

    void sa(int[MAX_DIM][MAX_DIM]) {

    }

    double solve() {
        auto start = chrono::high_resolution_clock::now();
        sa(result);
        auto stop = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = stop - start;
        return duration.count();
    }
};