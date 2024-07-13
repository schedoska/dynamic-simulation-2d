#include "Vector.h"
#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>

int Math::Vector::count = 0;

Math::Vector::Vector(float _x, float _y)
{
    this->x = _x;
    this->y = _y;
    count++;
}

Math::Vector Math::Vector::normal() const
{
    float len = length();
    return Vector(x / len, y / len);
}

float Math::Vector::length() const
{
    return sqrt(pow(x, 2) + pow(y, 2));
}

Math::Vector Math::Vector::operator+(const Vector& vector) const
{
    return Vector(x + vector.x, y + vector.y);
}

Math::Vector Math::Vector::operator-(const Vector& vector) const
{
    return Vector(x - vector.x, y - vector.y);
}

Math::Vector Math::Vector::operator*(const float scalar) const
{
    return Vector(x * scalar, y * scalar);
}

float Math::Vector::dotProduct(const Vector& vector) const
{
    return vector.x * x + vector.y * y;
}

Math::Vector Math::ApplyRotation(const Vector& vector, const float rad)
{
    Vector result;
    result.x = vector.x * cos(rad) - vector.y * sin(rad);
    result.y = vector.x * sin(rad) + vector.y * cos(rad);
    return result;
}

sf::Vector2f Math::ConvertToSFVector2F(const Vector& vector)
{
    return sf::Vector2f(vector.x, vector.y);
}

std::ostream& Math::operator<<(std::ostream& stream, const Math::Vector& vector)
{
    stream << "[" << vector.x << "," << vector.y << "]";
    return stream;
}

float Math::ConvertRadToDeg(float radians)
{
    return radians * (180.0 / M_PI);
}
