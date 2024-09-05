#include "Utils.h"

template <typename T>
inline sf::Vector2f utils::vec2_to_sfml(const vl::vec<T, 2>& v) {
    return sf::Vector2f(v[0], v[1]);
}

template<typename T>
inline bool utils::is_in_range(const T& val, T r1, T r2)
{
    if (r1 > r2) std::swap(r1, r2);
    if (val > r1 && val < r2) return true;
    else return false;
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

/*std::shared_ptr<ConvexDebug> utils::generate_rect(vl::vec2d pos, vl::vec2d size, double mass)
{
    std::shared_ptr<ConvexDebug> v(new ConvexDebug(pos));
    v->add_vertex(vl::vec2d(-size[0] / 2.f, -size[1] / 2.f));
    v->add_vertex(vl::vec2d(size[0] / 2.f, -size[1] / 2.f));
    v->add_vertex(vl::vec2d(size[0] / 2.f, size[1] / 2.f));
    v->add_vertex(vl::vec2d(-size[0] / 2.f, size[1] / 2.f));
    v->update_shape();
    v->mass() = mass;
    v->inertia() = (size[0] * size[0] + size[1] * size[1]) * mass / 12;

    return v;
}*/

vl::vec2d utils::rotate(const vl::vec2d& vec, const double& ang)
{
    double x = cos(ang) * vec[0] - sin(ang) * vec[1];
    double y = sin(ang) * vec[0] + cos(ang) * vec[1];
    return vl::vec2d(x, y);
}

double utils::angle(const vl::vec2d& ref, const vl::vec2d& sec)
{
    double dot = ref.dot(sec);
    double det = utils::cross(ref, sec);
    double angle = std::atan2(det, dot);

    return angle > 0 ? angle : 4.0 * std::acos(0.0) + angle;
}

double utils::angle2(const vl::vec2d& ref, const vl::vec2d& sec)
{
    double dot = ref.dot(sec);
    double det = utils::cross(ref, sec);
    return std::atan2(det, dot);
}

utils::segments_relation utils::check_relation(
    const vl::vec2d& start_a, 
    const vl::vec2d& end_a, 
    const vl::vec2d& start_b, 
    const vl::vec2d& end_b)
{
    double a1 = utils::cross(end_a - start_a, end_b - start_a);
    double a2 = utils::cross(end_a - start_a, start_b - start_a);
    double b1 = utils::cross(end_b - start_b, end_a - start_b);
    double b2 = utils::cross(end_b - start_b, start_a - start_b);

    if (a1 == 0 && a1 == 0 && b1 == 0 && b2 == 0) {
        vl::vec2d dir = end_a - start_a;
        double s_a = dir.dot(start_a);
        double e_a = dir.dot(end_a);
        double s_b = dir.dot(start_b);
        double e_b = dir.dot(end_b);
        if ((s_a == s_b && e_a == e_b) || (s_a == e_b && e_a == s_b))
            return utils::same;
        if (is_in_range(s_a, s_b, e_b) || is_in_range(e_a, s_b, e_b))
            return utils::intetsects;
        if (is_in_range(s_b, s_a, e_a) || is_in_range(e_b, s_a, e_a))
            return utils::intetsects;
        return utils::none;
    }
    if (a1 * a2 < 0 && b1 * b2 < 0) 
        return utils::intetsects;                              
    return utils::none;
}
