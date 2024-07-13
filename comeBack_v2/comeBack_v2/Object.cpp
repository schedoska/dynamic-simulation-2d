#include "Object.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

Object::Object(float x, float y)
{
	m_position = Math::Vector(x, y);
	m_force = Math::Vector(0,0);
	m_velocity = Math::Vector(0, 0);
	m_rotation = 0; // M_PI / 4;
	m_angularVelocity = 0;
	m_mass = 1;

	disableDebugDrawMode();
}

void Object::addVertex(Math::Vector vertex)
{
	m_vertexList.push_back(vertex);
	if (m_debugDrawMode) updateDebugVertex();
}

float Object::getRotation() const
{
	return m_rotation;
}

Math::Vector Object::getPosition() const
{
	return m_position;
}

void Object::setRotation(const float angle)
{
	m_rotation = angle;
	if (m_debugDrawMode) updateDebugPosition();
}

void Object::setPosition(const Math::Vector& position)
{
	m_position = position;
	if (m_debugDrawMode) updateDebugPosition();
}

void Object::setVelocity(const Math::Vector& velocity)
{
	m_velocity = velocity;
}

void Object::setForce(const Math::Vector& force)
{
	m_force = force;
}

void Object::setAngularVelocity(const float angularVelocity)
{
	m_angularVelocity = angularVelocity;
}

void Object::setMass(const float mass)
{
	m_mass = mass;
}

Math::Vector Object::getVelocity() const
{
	return m_velocity;
}

Math::Vector Object::getForce() const
{
	return m_force;
}

float Object::getAngularVelocity() const
{
	return m_angularVelocity;
}

float Object::getMass() const
{
	return m_mass;
}

void Object::update(float dt)
{
	m_velocity = m_velocity + (m_force * (1.0 / m_mass)) * dt;
	m_position = m_position + m_velocity * dt;
	m_rotation = m_rotation + m_angularVelocity * dt;

	if (m_debugDrawMode) updateDebugPosition();
}

Math::Vector Object::mapToWorld(Math::Vector localVector) const
{
	return Math::ApplyRotation(localVector, m_rotation) + m_position;
}

std::vector<Math::Vector>& Object::getVertexList()
{
	return m_vertexList;
}

void Object::printInfo()
{
	std::cout << "Position: " << m_rotation << "\n";
	std::cout << "Rotation: " << m_rotation << "\n";

	for (int i = 0; i < m_vertexList.size(); i++)
		std::cout << "Vertex (" << i << ") local position: " << m_vertexList[i]
		<< " |  global position: " << mapToWorld(m_vertexList[i]) << "\n";
}

void Object::debugDraw(sf::RenderWindow& window)
{
	if (m_debugDrawMode == false || m_drawableObject == nullptr) return;
	if (m_vertexList.size() < 3) return;
	window.draw(*m_drawableObject);
}

void Object::enableDebugDrawMode()
{
	m_debugDrawMode = true;
	m_drawableObject = new sf::ConvexShape();
	m_drawableObject->setFillColor(sf::Color::Transparent);
	m_drawableObject->setOutlineThickness(1);
	m_drawableObject->setOutlineColor(sf::Color::Cyan);
	updateDebugVertex();
	updateDebugPosition();
}

void Object::disableDebugDrawMode()
{
	m_debugDrawMode = false;
	if (m_drawableObject != nullptr)
		delete m_drawableObject;
	m_drawableObject = nullptr;
}

void Object::setCollidingState(bool colliding)
{
	m_collidingState = colliding;
	if (m_drawableObject == nullptr || m_debugDrawMode == false) return;
	if (m_collidingState) m_drawableObject->setFillColor(sf::Color::Red);
	else m_drawableObject->setFillColor(sf::Color::Transparent);
}

void Object::updateDebugVertex()
{
	if (m_drawableObject == nullptr || m_debugDrawMode == false) return;
	m_drawableObject->setPointCount(m_vertexList.size());
	for (int i = 0; i < m_vertexList.size(); i++)
	{
		m_drawableObject->setPoint(i, Math::ConvertToSFVector2F(m_vertexList[i]));
	}
}

void Object::updateDebugPosition()
{
	if (m_drawableObject == nullptr || m_debugDrawMode == false) return;
	m_drawableObject->setPosition(Math::ConvertToSFVector2F(m_position));
	m_drawableObject->setRotation(Math::ConvertRadToDeg(m_rotation));
}
