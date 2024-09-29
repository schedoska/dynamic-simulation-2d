#include "app.h"
#include "ds2/collision_solver.h"
#include "ds2/Utils.h"
#include "ds2/concave_shape.h"

app::app(sf::RenderWindow* window)
	:_window(window)
{
	ImGui::SFML::Init(*_window);

	ds2::convex_shape block;
	block.add(vl::vec2d(0, 0));
	block.add(vl::vec2d(50, 0));
	block.add(vl::vec2d(50, 50));
	block.add(vl::vec2d(0, 50));

	ds2::concave_shape s;
	s.add(vl::vec2d(-30, -100));
	s.add(vl::vec2d(30, -100));
	s.add(vl::vec2d(30, -30));
	s.add(vl::vec2d(90, -30));
	s.add(vl::vec2d(90, -100));
	s.add(vl::vec2d(120, -100));
	s.add(vl::vec2d(120, 100));
	s.add(vl::vec2d(-0, 100));
	s.add(vl::vec2d(-120, -130));

	body *b = new body("Blok 1");
	//b->shape().add(block);
	b->shape() = s.generate_shape_group(ds2::triangulation::delaunay);
	b->update_shape();
	b->pos() = { 250,200 };
	b->rot() = 0.3;

	body* b2 = new body(*b);

	_bodies.push_back(b);
	_bodies.push_back(b2);

	bh.set_target(b);
	oc_ui.set_target(&bh);
	//pt.start_shape(std::bind(&app::add_body, this, std::placeholders::_1));

	mt_ui.set_create_convex_cbck(std::bind(
		&app::add_convex_body, this, std::placeholders::_1));
	mt_ui.set_create_circle_cbck(std::bind(
		&app::add_circle_body, this, std::placeholders::_1, std::placeholders::_2));
}

app::~app()
{
	ImGui::SFML::Shutdown();
}

void app::update(const sf::Time& dt)
{
	ImGui::SFML::Update(*_window, dt);

	sf::Vector2f mouse_pos = (sf::Vector2f)sf::Mouse::getPosition(*_window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		body* b = body_at(utils::sfml_to_vec2d(mouse_pos));
		if (bh.target() == nullptr) {
			bh.set_target(b);
		}
		else if (b == nullptr){
			bh.set_target(nullptr);
		}
	}

	bh.update(_window);
	pt.update(_window);
}

void app::draw()
{
	for (auto& b : _bodies) b->draw(*_window);
	bh.draw(_window);
	pt.draw(_window);
	oc_ui.draw();
	mt_ui.draw();

	ImGui::ShowDemoWindow();

	ImGui::SFML::Render(*_window);
}

void app::add_convex_body(const std::vector<vl::vec2d>& vertices)
{
	ds2::convex_shape cs;
	for (const vl::vec2d& v : vertices) {
		cs.add(v);
	}
	body* b = new body("Object #" + std::to_string((int)_bodies.size() + 1));
	b->shape().add(cs);
	vl::vec2d ctr = b->shape().centroid();
	b->shape().translate(ctr * -1.0);
	b->pos() = ctr;
	b->update_shape();
	_bodies.push_back(b);
	bh.set_target(b);
}

void app::add_circle_body(const vl::vec2d& pos, const double& radius)
{
	ds2::circle_shape cs;
	cs.set_radius(radius);
	body* b = new body("Object #" + std::to_string((int)_bodies.size() + 1));
	b->shape().add(cs);

	b->pos() = pos;
	b->update_shape();
	_bodies.push_back(b);
	bh.set_target(b);
}

void app::add_concave_body(const std::vector<vl::vec2d> &vertices, bool delauney)
{
	ds2::concave_shape cs;
	for (const vl::vec2d& v : vertices) {
		cs.add(v);
	}
	body* b = new body("Object #" + std::to_string((int)_bodies.size() + 1));

	if (delauney) {
		b->shape() = cs.generate_shape_group(ds2::triangulation::delaunay);
	}
	else {
		b->shape() = cs.generate_shape_group(ds2::triangulation::expanding);
	}
	vl::vec2d ctr = b->shape().centroid();
	b->shape().translate(ctr * -1.0);
	b->pos() = ctr;
	b->update_shape();
	_bodies.push_back(b);
	bh.set_target(b);
}

body* app::body_at(const vl::vec2d& scene_pos)
{
	ds2::object pos_obj(scene_pos);
	pos_obj.shape().add(ds2::circle_shape());

	for (const auto& b : _bodies) {
		ds2::object_collision_data ocd = ds2::collision_detection::check(&pos_obj, b);
		if (ocd.data.collides) return b;
	}
	return nullptr;
}
