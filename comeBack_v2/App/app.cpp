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

	body *b = new body();
	//b->shape().add(block);
	b->shape() = s.generate_shape_group(ds2::triangulation::delaunay);
	b->update_shape();
	b->pos() = { 200,200 };

	_bodies.push_back(b);

	bh.set_target(b);
}

app::~app()
{
	ImGui::SFML::Shutdown();
}

void app::update(const sf::Time& dt)
{
	ImGui::SFML::Update(*_window, dt);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i p = sf::Mouse::getPosition(*_window);
		vl::vec2d vlp = utils::sfml_to_vec2d(p);
		body_at(vlp);
	}

	bh.update(_window);

	ImGui::Begin("Hello, world!");
	static float g = 0;
	ImGui::SliderFloat("Some value", &g, 0.0f, 100.0f);
	ImGui::End();
}

void app::draw()
{
	for (auto& b : _bodies) b->draw(*_window);
	bh.draw(_window);

	ImGui::SFML::Render(*_window);
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
