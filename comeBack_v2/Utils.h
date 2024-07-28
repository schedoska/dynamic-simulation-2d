#ifndef UTILS_HH
#define UTILS_HH

#include "vec2.h"
#include <SFML/Graphics.hpp> 

namespace utils {
    template <typename T>
    inline sf::Vector2f vec2_to_sfml(const vl::vec<T, 2>& v);

    template <typename T>
    bool is_in_bounds(const T& value, const T& low, const T& high);

    float RadToDegrees(const float radians);

    void drawLine(vl::vec2d start, vl::vec2d end, sf::RenderWindow& window, sf::Color color);

    void drawPoint(vl::vec2d point, sf::RenderWindow& window, sf::Color color);

    double cross(const vl::vec2d& a, const vl::vec2d& b);

    //std::shared_ptr<ConvexDebug> generate_rect(vl::vec2d pos, vl::vec2d size, double mass);

    vl::vec2d rotate(const vl::vec2d& vec, const double& ang);

    double angle(const vl::vec2d& ref, const vl::vec2d& sec);

    bool intersects(
        const vl::vec2d& start_a,
        const vl::vec2d& end_a,
        const vl::vec2d& start_b,
        const vl::vec2d& end_b);
}
#endif