#pragma once

#include <SFML/Graphics.hpp>

class DebugUI
{
public:
	DebugUI(float x = 0, float y = 0);
	void draw(sf::RenderWindow& window);
	void updateFPS(float fps);
	void updateSolverTime(float solverTime);
	void updateObjectCount(int objectCount);
	void updateText();

private:
	float m_fps;
	float m_solverTime;
	int m_objectCount;

	sf::Font m_font;
	sf::Text m_debugText;
};

