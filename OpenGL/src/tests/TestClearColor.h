#pragma once

#include "Test.h"

namespace Tests
{
	class TestClearColor final : public Test
	{
	public:
		TestClearColor();
		~TestClearColor() override = default;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];
	};
}