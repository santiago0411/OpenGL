#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace Tests
{
	class Test
	{
	public:
		virtual ~Test() = default;
		
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu final : public Test
	{
	public:
		TestMenu(Test*& currentTestPtr);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test '" << name << "'\n";
			m_Tests.push_back({ name, []() {return new T(); } });
		}

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};
}