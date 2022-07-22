#include <iostream>
#include <fstream>
#include <vector>
#include <recursive.h>
#include "backtracking.h"
#include "sa.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

using namespace std;
const char* glsl_version = "#version 130";
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

vector<vector<int>> osudoku;
vector<vector<bool>> is_fixed;
vector<vector<int>> result = osudoku;
bool* finished = new bool(false);

bool show_close = false;
void get_input() {
    ifstream Input("src/input.txt");
    string temp;
    while (getline(Input, temp)) {
        vector<int> row;
        vector<bool> frow;
        while (temp.length() != 0) {
            row.push_back(temp[0] - '0');
            if (temp[0] - '0') {
                frow.push_back(true);
            } else {
                frow.push_back(false);
            }
            temp.erase(0, 2);
        }
        osudoku.push_back(row);
        is_fixed.push_back(frow);
    }
    Input.close();
}

void output(vector<vector<int>> sudoku) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            cout << sudoku.at(row).at(col) << " ";
        }
        cout << endl;
    }
}

int main(int argc, char const *argv[])
{
    get_input();
	SA solver_sa;
	Backtracking solver_bt;
	
	recursive res;

	// Creating Window
	if (!glfwInit()) {
		return 1;
	}

	GLFWwindow *window = glfwCreateWindow(900, 600, "Testing", NULL, NULL);
	if (window == NULL) {
		return 1;
	}
	glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();
    ImGuiStyle& style = ImGui::GetStyle();

    ImFont* myfont = io.Fonts->AddFontFromFileTTF("fonts/MesloLGMRegular.ttf", 20.0f);
    io.Fonts->Build();
    style.ScaleAllSizes(2);

	// Main Loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		if (*finished == false) {
            result = solver_bt.solve(osudoku, is_fixed, finished, window);
        } else {
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
        }
	}
	
	// Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
