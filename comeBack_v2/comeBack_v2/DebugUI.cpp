#include "DebugUI.h"
#include <string>

DebugUI::DebugUI(float x, float y)
{
	m_debugText.setPosition(x, y);

	m_fps = 0;
	m_objectCount = 0;
	m_solverTime = 0;

	m_font.loadFromFile("Resources\\OpenSans-Regular.ttf");
	m_debugText.setFont(m_font);
	m_debugText.setString("Hello world\nwif3");
	m_debugText.setCharacterSize(20);
}

void DebugUI::draw(sf::RenderWindow& window)
{
	window.draw(m_debugText);
}

void DebugUI::updateFPS(float fps)
{
	m_fps = fps;
}

void DebugUI::updateSolverTime(float solverTime)
{
	m_solverTime = solverTime;
}

void DebugUI::updateObjectCount(int objectCount)
{
	m_objectCount = objectCount;
}

void DebugUI::updateText()
{
	m_debugText.setString(std::string("2D Physics Engine - v0.1\n") +
		std::string("Object count: ") + std::to_string(m_objectCount) +
		std::string("\nFPS: ") + std::to_string(m_fps) +
		std::string("\nSolver time (ms): ") + std::to_string(m_solverTime));
}
