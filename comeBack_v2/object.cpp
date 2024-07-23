#include "object.h"
#include "mat22.h"
#include "Utils.h"

ds2::object::object(){
    init();
}

ds2::object::object(const vl::vec2d& pos) {
    init();
    _pos = vl::vec2d(pos);
}

vl::vec2d& ds2::object::pos(){
    return _pos;
}

const vl::vec2d& ds2::object::pos() const{
    return _pos;
}

vl::vec2d& ds2::object::vel() {
    return _vel;
}

const vl::vec2d& ds2::object::vel() const {
    return _vel;
}

double& ds2::object::mass() {
    return _mass;
}

const double& ds2::object::mass() const {
    return _mass;
}

double& ds2::object::rot()
{
    return _rot;
}

const double& ds2::object::rot() const
{
    return _rot;
}

double& ds2::object::rot_vel()
{
    return _rot_vel;
}

const double& ds2::object::rot_vel() const
{
    return _rot_vel;
}

double& ds2::object::inertia()
{
    return _inertia;
}

const double& ds2::object::inertia() const
{
    return _inertia;
}

void ds2::object::update(const double& dt)
{
    //_vel += _acc * dt;
    // _rot_vel += _rot_acc * dt;

    _pos += _vel * dt;
    _rot += _rot_vel * dt;
}

vl::vec2d ds2::object::global(const vl::vec2d& local)
{
    double x = cos(_rot) * local[0] - sin(_rot) * local[1] + _pos[0];
    double y = sin(_rot) * local[0] + cos(_rot) * local[1] + _pos[1];
    return vl::vec2d(x, y);
}

vl::vec2d ds2::object::local(const vl::vec2d& global)
{
    vl::vec2d v = global - pos();
    double x = cos(-_rot) * v[0] - sin(-_rot) * v[1];
    double y = sin(-_rot) * v[0] + cos(-_rot) * v[1];
    return vl::vec2d(x, y);
}

void ds2::object::apply_force(const vl::vec2d& force, const vl::vec2d& point, const double& dt)
{
    _vel += (force / _mass) * dt;
    _rot_vel += (vl::cross(point, local(force) + pos()) / _inertia) * dt;
    //_rot_vel += (vl::cross(point, utils::rotate(force, _rot)) / inertia) * dt;
    //std::cout << (vl::cross(point, local(force) + pos()) / inertia) * dt << "\n";
}

void ds2::object::init()
{
    _pos = vl::vec2d();
    _mass = 1;
    _inertia = 1;
}

ds2::shape::shape(const vl::vec2d& loc_pos)
    : _loc_pos(loc_pos) {}

ds2::shape::~shape() {}

const vl::vec2d& ds2::shape::loc_pos() const
{
    return _loc_pos;
}

vl::vec2d& ds2::shape::loc_pos()
{
    return _loc_pos;
}

ds2::circle_shape::circle_shape(const vl::vec2d& loc_pos)
    : ds2::shape(loc_pos)
{

}

const double& ds2::circle_shape::radius() const
{
    return _radius;
}

double& ds2::circle_shape::radius()
{
    return _radius;
}

ds2::convex_shape::convex_shape(const vl::vec2d& loc_pos)
    : shape(loc_pos) {}

void ds2::convex_shape::add(const vl::vec2d& vertex)
{
    _vertices.push_back(vertex);
}

void ds2::convex_shape::clear()
{
    _vertices.clear();
}

const std::vector<vl::vec2d>& ds2::convex_shape::vertices() const
{
    return _vertices;
}

void ds2::shape_group::add(const circle_shape& circle)
{
    _circles.push_back(circle);
}

void ds2::shape_group::add(const convex_shape& convex)
{
    _convexes.push_back(convex);
}

const std::vector<ds2::circle_shape>& ds2::shape_group::circles() const
{
    return _circles;
}

const std::vector<ds2::convex_shape>& ds2::shape_group::convexes() const
{
    return _convexes;
}

void ds2::shape_group::clear()
{
    _circles.clear();
    _convexes.clear();
}
