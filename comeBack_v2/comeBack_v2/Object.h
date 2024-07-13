#pragma once

#include "Vector.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Object
{
public:
	Object(float x = 0, float y = 0);
	void addVertex(Math::Vector vertex);

	float getRotation() const;
	Math::Vector getPosition() const;
	void setRotation(const float angle); // In radians
	void setPosition(const Math::Vector& position);

	void setVelocity(const Math::Vector& velocity);
	void setForce(const Math::Vector& force);
	void setAngularVelocity(const float angularVelocity);
	void setMass(const float mass);
	Math::Vector getVelocity() const;
	Math::Vector getForce() const;
	float getAngularVelocity() const;
	float getMass() const;
	void update(float dt);

	Math::Vector mapToWorld(Math::Vector localVector) const;
	std::vector<Math::Vector>& getVertexList();

	void printInfo();

	void debugDraw(sf::RenderWindow &window);
	void enableDebugDrawMode();
	void disableDebugDrawMode();
	void setCollidingState(bool colliding);

private:
	Math::Vector m_position;
	float m_rotation; // In radians
	std::vector<Math::Vector> m_vertexList;

	bool m_debugDrawMode;
	sf::ConvexShape* m_drawableObject;
	void updateDebugVertex();
	void updateDebugPosition();

	Math::Vector m_velocity;
	Math::Vector m_force;
	float m_angularVelocity;
	float m_mass;

	bool m_collidingState;
};

