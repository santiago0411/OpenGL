#include "Test.h"

#include "imgui/imgui.h"

namespace Tests
{
	TestMenu::TestMenu(Test*& currentTestPtr)
		: m_CurrentTest(currentTestPtr)
	{
	}
	
	void TestMenu::OnImGuiRender()
	{
		for (auto& [name, func]: m_Tests)
		{
			if (ImGui::Button(name.c_str()))
				m_CurrentTest = func();
		}
	}
}
