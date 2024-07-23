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

double utils::cross(const vl::vec2d& a, const vl::vec2d& b)
{
    return a[0] * b[1] - a[1] * b[0];
}

std::shared_ptr<ConvexDebug> utils::generate_rect(vl::vec2d pos, vl::vec2d size, double mass)
{
    std::shared_ptr<ConvexDebug> v(new ConvexDebug(pos));
    v->add_vertex(vl::vec2d(-size[0] / 2.f, -size[1] / 2.f));
    v->add_vertex(vl::vec2d(size[0] / 2.f, -size[1] / 2.f));
    v->add_vertex(vl::vec2d(size[0] / 2.f, size[1] / 2.f));
    v->add_vertex(vl::vec2d(-size[0] / 2.f, size[1] / 2.f));
    v->update_shape();
    v->mass() = mass;
    v->inertia = (size[0] * size[0] + size[1] * size[1]) * mass / 12;

    return v;
}

vl::vec2d utils::rotate(const vl::vec2d& vec, const double& ang)
{
    double x = cos(ang) * vec[0] - sin(ang) * vec[1];
    double y = sin(ang) * vec[0] + cos(ang) * vec[1];
    return vl::vec2d(x, y);
}
