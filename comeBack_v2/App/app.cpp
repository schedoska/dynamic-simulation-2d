#include "app.h"
#include "ds2/collision_solver.h"
#include "ds2/Utils.h"
#include "ds2/concave_shape.h"
#include <fstream>
#include "json_utils.h"

app::app(sf::RenderWindow* window)
	:_window(window), _mode(app_mode::edition)
{
	ImGui::SFML::Init(*_window);

	bh.set_target(nullptr);
	oc_ui.set_target(&bh);
	//pt.start_shape(std::bind(&app::add_body, this, std::placeholders::_1));

	mt_ui.set_create_convex_cbck(std::bind(
		&app::add_convex_body, this, std::placeholders::_1));
	mt_ui.set_create_circle_cbck(std::bind(
		&app::add_circle_body, this, std::placeholders::_1, std::placeholders::_2));

	sim_ui.set_start_sim_cbck(std::bind(&app::start_simulation, this));
	sim_ui.set_restart_sim_cbck(std::bind(&app::restart_simulation, this));
	sim_ui.set_scene(&_scene);

	//return;

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
	b->pos() = { 450,200 };

	_bodies.push_back(b);
	_bodies.push_back(b2);
	_scene.add(b);
	_scene.add(b2);
	//_scene.add_object(b2);
	b->vel() = { -10,0 };

	
	dble_spring* sd = new dble_spring(nullptr, nullptr, { 300,300 }, { 500,300 });
	_joints.push_back(sd);
	_dwbl_joints.push_back(sd);
	_scene.add(sd);
}

app::~app()
{
	ImGui::SFML::Shutdown();
}

void app::update(const sf::Time& dt)
{
	ImGui::SFML::Update(*_window, dt);
	sim_ui.set_fps(1.f / (float)dt.asSeconds(), 40);

	if (_mode == app_mode::edition) {
		edition_update(dt);
	}
	else {
		simulation_update(dt);
	}
}

void app::simulation_update(const sf::Time& dt)
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(*_window);
	bool left_mouse_btn = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	ft.update(_window, dt);

	if (left_mouse_btn && ImGui::GetIO().WantCaptureMouse == false) {
		if (!ft.target()) {
			body* b = body_at(utils::sfml_to_vec2d(mouse_pos));
			ft.set_target(b, mouse_pos);
		}
	}
	for (auto& i : _bodies) {
		if (i->mass() != ds2::inf_mass)
		i->vel() += vl::vec2d(0, 100 * dt.asSeconds());
	}
	_scene.update(sim_ui.step_time() / 1000.0, *_window);
}

void app::edition_update(const sf::Time& dt)
{
	sf::Vector2f mouse_pos = (sf::Vector2f)sf::Mouse::getPosition(*_window);
	bool left_mouse_btn = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool left_ctrl_btn = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
	bool delete_btn = sf::Keyboard::isKeyPressed(sf::Keyboard::Delete);
	bool c_btn = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
	bool v_btn = sf::Keyboard::isKeyPressed(sf::Keyboard::V);

	bh.update(_window);
	jh.update(_window);
	pt.update(_window);

	if (left_mouse_btn && ImGui::GetIO().WantCaptureMouse == false) {
		if (!jh.is_active() && !bh.is_active()) 
		{
			joint_at_data jad = joint_at(utils::sfml_to_vec2d(mouse_pos));
			body* b = body_at(utils::sfml_to_vec2d(mouse_pos));

			if (jh.target() && left_ctrl_btn) {
				jh.target_set_object(b);
				return;
			}

			if (jad.joint) {
				jh.set_target(jad.joint, jad.type);
				jc_ui.set_target(jad.joint);
				bh.set_target(nullptr);
			}
			else if (b) {
				bh.set_target(b);
				jh.set_target(nullptr);
				jc_ui.set_target(nullptr);
			}
			else {
				bh.set_target(nullptr);
				jh.set_target(nullptr);
				jc_ui.set_target(nullptr);
			}
		}
	}
	if (delete_btn) {
		remove(bh.target());
		bh.set_target(nullptr);
	}
	if (left_ctrl_btn && c_btn) {
		buffor_body = bh.target();
	}
	static bool block = false;
	if (left_ctrl_btn && v_btn && !block) {
		if (buffor_body) {
			body* b = new body(*buffor_body);
			b->pos() = utils::sfml_to_vec2d(mouse_pos);
			b->update_shape();
			_bodies.push_back(b);
			_scene.add(b);
			bh.set_target(b);
		}
		block = true;
	}
	if (!(left_ctrl_btn && v_btn)) block = false;
}

void app::draw()
{
	for (auto& i : _bodies) i->draw(*_window);

	if (_mode == app_mode::edition) {
		bh.draw(_window);
		jh.draw(_window);
		pt.draw(_window);
		oc_ui.draw();
		mt_ui.draw();
		jc_ui.draw();
	}
	else {
		ft.draw(_window);
	}
	sim_ui.draw();

	for (auto& i : _dwbl_joints) i->draw(*_window);

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

void app::remove(const body* b)
{
	if (b == nullptr) return;
	_bodies.erase(std::remove(_bodies.begin(), _bodies.end(), b));
	_scene.remove(b);
	delete b;
}

void app::start_simulation()
{
	_mode = app_mode::simulation;
	save_json("C:\\Users\\chedo\\OneDrive\\Pulpit\\test.json");
}

void app::restart_simulation()
{
	_mode = app_mode::edition;
	load_json("C:\\Users\\chedo\\OneDrive\\Pulpit\\test.json");
}

void app::set_step_time(const float& st)
{
	_step_time = st;
}

void app::save_json(const std::string& path)
{
	std::ofstream ofs(path);
	nlohmann::json json_obj;

	for (const body* it : _bodies) {
		json_obj["bodies"].push_back(json_utils::serialize(*it));
	}
	ofs << std::setw(3) << json_obj;
	ofs.close();
}

void app::load_json(const std::string& path)
{
	std::ifstream ifs(path);
	nlohmann::json json_obj;
	ifs >> json_obj;

	jh.set_target(nullptr);
	bh.set_target(nullptr);
	_scene.remove_all();
	for (auto* b : _bodies) {
		delete b;
	}
	for (auto* j : _joints) {
		delete j;
	}
	_bodies.clear();
	_joints.clear();

	for (const auto& i : json_obj["bodies"]) {
		// iteracja po kolejnych obiektach
		body* b = new body(json_utils::deserialize_body(i));
		b->update_shape();
		_bodies.push_back(b);
		_scene.add(b);
	}
	ifs.close();
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

body* app::body_at(const vl::vec2d& scene_pos) const
{
	ds2::object pos_obj(scene_pos);
	pos_obj.shape().add(ds2::circle_shape());

	for (const auto& b : _bodies) {
		ds2::object_collision_data ocd = ds2::collision_detection::check(&pos_obj, b);
		if (ocd.data.collides) return b;
	}
	return nullptr;
}

joint_at_data app::joint_at(const vl::vec2d& scene_pos) const
{
	for (ds2::joint* i : _joints) {
		ds2::joint_type jt = i->type();
		double dist_a = (scene_pos - i->global_a()).len();
		double dist_b = (scene_pos - i->global_b()).len();

		if (jt == ds2::joint_type::hinge || jt == ds2::joint_type::motor) {
			if (dist_a < 50) return joint_at_data{ i, joint_handler_mode::both };
		}
		else {
			if (dist_a < 50) return joint_at_data{ i, joint_handler_mode::a };
			if (dist_b < 50) return joint_at_data{ i, joint_handler_mode::b };
		}
	}
	return joint_at_data{ nullptr, joint_handler_mode::a };
}


