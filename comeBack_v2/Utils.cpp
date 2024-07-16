#include "Utils.h"

template <typename T>
inline sf::Vector2f utils::vec2_to_sfml(const vl::vec<T, 2>& v) {
    return sf::Vector2f(v[0], v[1]);
}

float utils::RadToDegrees(const float radians)
{
    const double m_pi = acos(-1);
    return (radians * 180.0) / m_pi;
}

void utils::drawLine(vl::vec2d start, vl::vec2d end, sf::RenderWindow& window, sf::Color color)
{
    static sf::RectangleShape line;
    line.setPosition(vec2_to_sfml(start));
    vl::vec2d delta = start - end;

    line.setSize(sf::Vector2f(delta.len(), 2));
    float angle = RadToDegrees(atan(delta[1] / delta[0]));
    if (end[0] <= start[0]) angle -= 180;
    line.setRotation(angle);

    line.setFillColor(color);
    window.draw(line);
}

void utils::drawPoint(vl::vec2d point, sf::RenderWindow& window, sf::Color color)
{
    static sf::CircleShape pointDrawable;
    pointDrawable.setOrigin(2, 2);
    pointDrawable.setPosition(vec2_to_sfml(point));
    pointDrawable.setRadius(4);
    pointDrawable.setFillColor(color);

    window.draw(pointDrawable);
}