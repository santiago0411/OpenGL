#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

using namespace Tests;

int main()
{
    if (!glfwInit())
        return -1;    

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
    GLFWwindow* window = glfwCreateWindow(960, 540, "OpenGL Tests", nullptr, nullptr);
	
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!\n";

    std::cout << glGetString(GL_VERSION) << "\n";
	
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        Test* currentTest = nullptr;
        auto testMenu = new TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<TestClearColor>("Clear Color");
        testMenu->RegisterTest<TestTexture2D>("2D Texture");
    	
        while (!glfwWindowShouldClose(window))
        {
            Renderer::SetBackgroundColor({ 0.0f, 0.0f, 0.0f, 1.0f });
            Renderer::Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
            	
            	if (currentTest != testMenu && ImGui::Button("<-"))
            	{
                    delete currentTest;
                    currentTest = testMenu;
            	}
            	
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        	
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
	
    return 0;
}
