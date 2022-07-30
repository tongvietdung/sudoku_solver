#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "algorithms\SA.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 650;
const unsigned int SCR_HEIGHT = 500;
static int sudoku_input[9 * 9];
static std::vector<std::vector<int>> sudoku;
std::vector<std::vector<int>> result;
static bool is_fixed[9][9];
bool parsed = false;

void Solve() {
    // parse input
    if (!parsed) {
        int row, col;
        for (int cell = 0; cell < 9 * 9; cell++)
        {
            row = cell / 9;
            col = cell % 9;
            if (sudoku_input[cell] == 0)
            {
                sudoku.at(row).at(col) = 0;
            }
            else {
                sudoku.at(row).at(col) = sudoku_input[cell];
            }
        }
        parsed = true;
    }

    SA sa_algo(sudoku, is_fixed);
    result = sa_algo.Solve();
}

void show_sudoku_input() {
    ImGui::Begin("Sudoku Input##1");
        ImGui::BeginTable("sudoku_input", 9, ImGuiTableFlags_Borders + ImGuiTableFlags_NoPadInnerX + ImGuiTableFlags_NoPadOuterX);
        for (int cell = 0; cell < 9 * 9; cell++)
        {
            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushID(cell);
            ImGui::DragInt("##cell", &sudoku_input[cell], 1.0f, 0, 9);
            ImGui::PopID();
        }
        ImGui::EndTable();

        if (ImGui::Button("Solve Sudoku")) {
            Solve();
        }
    ImGui::End();
}

void show_sudoku_result() {
    ImGui::Begin("Result##1");
    ImGui::BeginTable("sudoku", 9, ImGuiTableFlags_Borders + ImGuiTableFlags_NoPadInnerX + ImGuiTableFlags_NoPadOuterX + ImGuiTableFlags_RowBg);
    int row, col;
    for (int cell = 0; cell < 9 * 9; cell++)
    {
        ImGui::TableNextColumn();
        row = cell / 9;
        col = cell % 9;

        // Fill with button and change color
        ImGui::PushID(cell);
        if (is_fixed[row][col]) {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.5f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.5f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.5f, 0.8f, 0.8f));
            ImGui::Button(std::to_string(result[row][col]).c_str(), ImVec2(-FLT_MIN, 0.0f));
            ImGui::PopStyleColor(3);
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
            ImGui::Button(std::to_string(result[row][col]).c_str(), ImVec2(-FLT_MIN, 0.0f));
            ImGui::PopStyleColor(3);
        }
        ImGui::PopID();

    }
    ImGui::EndTable();
    ImGui::End();
}

void Init() {
    std::ifstream initFile("sudoku.ini");
    std::string temp;
    int row = 0;
    int col;
    int count = 0;
    while (getline(initFile, temp)) {
        std::vector<int> rowInput;
        col = 0;
        while (temp.length() != 0) {
            rowInput.push_back(temp[0] - '0');
            sudoku_input[count] = temp[0] - '0';
            if (temp[0] - '0') {
                is_fixed[row][col] = true;
            }
            else {
                is_fixed[row][col] = false;
            }
            temp.erase(0, 2);
            col++;
            count++;
        }
        sudoku.push_back(rowInput);
        result.push_back(rowInput);
        row++;
    }
}

int main() {
    Init();

	// Setup Window
	if (!glfwInit()) return 1;

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sudoku Solver", NULL, NULL);
	if (window == NULL) return 1;
    glfwSetWindowPos(window, 500, 200);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable Vsync


	// Setup Imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.0f);

    ImFont* myfont =  io.Fonts->AddFontFromFileTTF(".\\dependencies\\imgui\\misc\\fonts\\Roboto-Medium.ttf", 20.0f);
    ImGuiStyle style = ImGui::GetStyle();
    //style.ScaleAllSizes(ImGui::GetFontSize());

	// Main loop
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        #pragma region Docking setup
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", nullptr, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
        #pragma endregion

        show_sudoku_input();

        show_sudoku_result();

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}