#include "TestTexture2D.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Tests
{
	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 960.f, 0.0f, 540.0f, -1.0f, 1.0f)),
			m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0))),
			m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f, // 0 Bottom left
            200.0f, 100.0f, 1.0f, 0.0f, // 1 Bottom right
            200.0f, 200.0f, 1.0f, 1.0f, // 2 Top right 
            100.0f, 200.0f, 0.0f, 1.0f, // 3 Top left
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
		
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
		
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
		
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		
        m_Texture = std::make_unique<Texture>("res/textures/fuente.png");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_Texture->Bind();
		
        {
            const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            const glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            Renderer::Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            const glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            const glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            Renderer::Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRender()
	{	
        ImGui::SliderFloat3("Translate A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translate B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
