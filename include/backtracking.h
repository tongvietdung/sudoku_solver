#include <thread>
#include <chrono>

using namespace std;
class Backtracking
{
private:
    vector<vector<int>> result;
    bool show_close = false;
    // Find next unassigned cell and save row and col
    bool next_unassigned_cell(vector<vector<int>> sudoku, int& row, int &col) {
        for (row = 0; row < 9; row++) {
            for (col = 0; col < 9; col++) {
                if (sudoku[row][col] == 0) 
                    return true;
            }
        }
        return false;
    }

    // Check if a candidate is safed to assigned to cell
    bool is_valid(vector<vector<int>> sudoku, int row, int col, int candidate) {
        // Check empty cell
        if (sudoku[row][col] != 0) return false;
        // Check row and column
        for (int i = 0; i < 9; i++) {
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
    bool backtracking(vector<vector<int>> sudoku, vector<vector<bool>> is_fixed, GLFWwindow* window) {
        int row, col;

        // Draw the sudoku
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Feed inputs to Dear ImGui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Do your thang here
        ImGui::Begin("Backtracking Algorithm", &show_close, ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::BeginGroup();
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Sudoku");
                if (ImGui::BeginTable("solution", 9, ImGuiTableFlags_Borders + ImGuiTableFlags_NoHostExtendX)) {
                    for (int row = 0; row < 9; row++) {
                        ImGui::TableNextRow();
                        string rowStr = "";
                        for (int col = 0; col < 9; col++) {
                            ImGui::TableSetColumnIndex(col);
                            if (is_fixed[row][col]) {
                                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), to_string(result[row][col]).c_str());
                            } else {
                                ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), to_string(result[row][col]).c_str());
                            }

                            // ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.9f, 0.89f, 0.86f, 0.5f));
                            // ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                        }
                    }
                    ImGui::EndTable();
                }
            ImGui::EndGroup();
        ImGui::End();

		// // Rendering
		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);

        this_thread::sleep_for(chrono::milliseconds(100));

        // Complete the sudoku with no unassigned cell
        if (!next_unassigned_cell(sudoku, row, col)) {
            return true;
        }

        // Test possible candidates from 1-9
        for (int candidate = 1; candidate <= 9; candidate++) {
            if (is_valid(sudoku, row, col, candidate)) {
                // If candidate is valid, assigned to cell
                sudoku[row][col] = candidate;
                result = sudoku;
                // Recursive find next solution from current one
                if (backtracking(sudoku, is_fixed, window)) {
                    return true;
                }
                sudoku[row][col] = 0;
            }
        }
        return false;
    }

public:
    vector<vector<int>> solve(vector<vector<int>> sudoku, vector<vector<bool>> is_fixed, bool* finished, GLFWwindow* window) {
        result = sudoku;
        backtracking(result, is_fixed, window);
        *finished = true;
        return result;
    }
};

