#ifndef UTILS_HH
#define UTILS_HH

#include "vec2.h"
#include <SFML/Graphics.hpp>

namespace utils {
    template <typename T>
    inline sf::Vector2f vec2_to_sfml(const vl::vec<T, 2>& v);

    float RadToDegrees(const float radians);

    void drawLine(vl::vec2d start, vl::vec2d end, sf::RenderWindow& window, sf::Color color);

    void drawPoint(vl::vec2d point, sf::RenderWindow& window, sf::Color color);

    double cross(const vl::vec2d& a, const vl::vec2d& b);
}

#endif