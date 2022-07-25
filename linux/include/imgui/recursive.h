#include <GL/gl.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
class recursive
{

public:
    int add(int s, GLFWwindow* window) {
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Feed inputs to Dear ImGui, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// Do your thang here
        ImGui::Begin("Testing");
            ImGui::Text(std::to_string(s).c_str());
        ImGui::End();


		// // Rendering
		ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
        
        if (s > 9) {
            return s;
        }
        s++ ;
        return add(s, window);
    }
};

