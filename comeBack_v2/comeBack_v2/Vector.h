#pragma once

#include <SFML/Graphics.hpp>

namespace Math
{
	class Vector
	{
	public:
		float x, y;
		Vector(float _x = 0, float _y = 0);
		Vector normal() const;
		float length() const;
		Vector operator+(const Vector& vector) const;
		Vector operator-(const Vector& vector) const;
		Vector operator*(const float scalar) const;
		float dotProduct(const Vector& vector) const;
		static int count;
	};

	Vector ApplyRotation(const Vector& vector, const float rad);
	sf::Vector2f ConvertToSFVector2F(const Vector& vector);
	std::ostream& operator<< (std::ostream& stream, const Math::Vector& vector);
	float ConvertRadToDeg(float radians);
}


