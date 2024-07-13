#pragma once

#include "Object.h"
#include <vector>

struct AxisProjectionData {
	float maxVal;
	float minVal;
};

struct BouncingRectangleArea
{
	BouncingRectangleArea(float _x = 0, float _y = 0, float _width = 0, float _height = 0);
	bool contains(const Math::Vector& vector);
	float x;
	float y;
	float width;
	float height;
};

class Scene
{
public:
	Scene();
	void addObject(Object* object);
	void checkCollisions();
	void setBouncingArea(const BouncingRectangleArea bouncingArea);
	void enableBouncingArea();
	void disableBouncingArea();

	void update(float dt);
	void debugDraw(sf::RenderWindow &window);

private:
	bool checkCollisionSAT(Object* objectA, Object* objectB);
	std::vector<Object*> m_objects;
	AxisProjectionData projectObjectOnAxis(Object* object, const Math::Vector& axis);
	BouncingRectangleArea m_bouncingAreaRectangle;
	bool m_bouncingArea;
};

