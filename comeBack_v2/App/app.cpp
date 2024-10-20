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

	_current_id = 0;
	bh.set_target(nullptr);
	oc_ui.set_target(&bh);

	mt_ui.set_polygon_tool(&pt);
	mt_ui.set_create_body_cbck(std::bind(
		&app::create_body, this, std::placeholders::_1, std::placeholders::_2));
	mt_ui.set_create_joint_cbck(std::bind(
		&app::create_joint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	mt_ui.set_remove_all_cbck(std::bind(&app::remove_all, this));
		
	sim_ui.set_start_sim_cbck(std::bind(&app::start_simulation, this));
	sim_ui.set_restart_sim_cbck(std::bind(&app::restart_simulation, this));
	sim_ui.set_scene(&_scene);

	pt.set_create_body_cbck(std::bind(
		&app::create_body, this, std::placeholders::_1, std::placeholders::_2));

	load_json("C:\\Users\\chedo\\OneDrive\\Pulpit\\test.json");
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
		body* b = body_at(utils::sfml_to_vec2d(mouse_pos));
		ft.set_target(b, mouse_pos);
	}

	_scene.set_gravity_v(sim_ui.gravity_v());

	// DRAG FRICTION
	for (auto& i : _bodies) {
		if (i->mass() != ds2::inf_mass) {}
			//i->vel() -= i->vel() * 0.1;
	}

	int ips = sim_ui.ips();
	double st = sim_ui.step_time() / 1000.0; // in seconds
	for (int i = 0; i < ips; ++i) {
		_scene.update(st, *_window);
	}
}

void app::edition_update(const sf::Time& dt)
{
	sf::Vector2f mouse_pos = (sf::Vector2f)sf::Mouse::getPosition(*_window);
	vl::vec2d mouse_pos_vec2d = utils::sfml_to_vec2d(mouse_pos);
	bool left_mouse_btn = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	bool left_ctrl_btn = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
	bool delete_btn = sf::Keyboard::isKeyPressed(sf::Keyboard::Delete);
	bool c_btn = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
	bool v_btn = sf::Keyboard::isKeyPressed(sf::Keyboard::V);

	pt.update(_window);
	if (pt.active()) return;

	bh.update(_window);
	jh.update(_window);

	if (left_mouse_btn && ImGui::GetIO().WantCaptureMouse == false) {
		if (!jh.is_active() && !bh.is_active()) 
		{
			joint_at_data jad = joint_at(utils::sfml_to_vec2d(mouse_pos));
			body* b = body_at(utils::sfml_to_vec2d(mouse_pos));

			if (jh.target() && left_ctrl_btn) {
				jh.target_set_object(b);
				return;
			}

			if (jad.dble_joint) {
				jh.set_target(jad.dble_joint, jad.type);
				jc_ui.set_target(jad.dble_joint);
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

	/* Deleting of selected joints and bodies */
	if (delete_btn) {
		remove(bh.target());
		remove(jh.target());
		bh.set_target(nullptr);
		jh.set_target(nullptr);
		jc_ui.set_target(nullptr);
	}

	/* Copy paste mechnism */
	if (left_ctrl_btn && c_btn) {
		_buffor_body = bh.target();
		_buffor_joint = jh.target();
	}
	static bool block = false;
	if (left_ctrl_btn && v_btn && !block) {
		if (_buffor_body) {
			create_body_cpy(*_buffor_body, mouse_pos_vec2d);
		}
		else if (_buffor_joint) {
			create_joint_cpy(*_buffor_joint, mouse_pos_vec2d, mouse_pos_vec2d + vl::vec2d(50, 0));
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

	for (auto& i : _dble_joints) i->draw(*_window);

	ImGui::ShowDemoWindow();
	ImGui::SFML::Render(*_window);
}

void app::create_body(const ds2::shape_group& shape, const vl::vec2d& pos)
{
	std::string name = "Object #" + std::to_string((int)_bodies.size() + 1);
	body* b = new body(++_current_id, name, shape, pos);
	_bodies.push_back(b);
	bh.set_target(b);
}

void app::create_body_cpy(const body& original, const vl::vec2d& pos)
{
	std::string name = "Object #" + std::to_string((int)_bodies.size() + 1);
	body* b = new body(original);
	b->set_id(++_current_id);
	b->pos() = pos;
	_bodies.push_back(b);
	bh.set_target(b);
}

void app::create_joint(ds2::joint_type type, const vl::vec2d& pos_a, const vl::vec2d& pos_b)
{
	dble_joint* j;

	switch (type) {
	case ds2::joint_type::spring:
		j = new dble_spring(nullptr, nullptr, pos_a, pos_b);
		break;
	case ds2::joint_type::hinge:
		j = new dble_hinge(nullptr, nullptr, pos_a, pos_b);
		break;
	case ds2::joint_type::motor:
		j = new dble_motor(nullptr, nullptr, pos_a, pos_b);
		break;
	}
	_dble_joints.push_back(j);
}

void app::create_joint_cpy(dble_joint& original, const vl::vec2d& pos_a, const vl::vec2d& pos_b)
{
	dble_joint* j;
	switch (original.joint()->type()) {
	case ds2::joint_type::spring:
		j = dynamic_cast<dble_spring*>(&original)->create_copy();
		break;
	case ds2::joint_type::hinge:
		j = dynamic_cast<dble_hinge*>(&original)->create_copy();
		break;
	case ds2::joint_type::motor:
		j = dynamic_cast<dble_motor*>(&original)->create_copy();
		break;
	}
	_dble_joints.push_back(j);
	j->set_body_a(nullptr);
	j->set_body_b(nullptr);
	j->joint()->set_loc_a(pos_a);
	j->joint()->set_loc_b(pos_b);
	
	sf::Vector2f mouse_pos = (sf::Vector2f)sf::Mouse::getPosition(*_window);
	joint_at_data jad = joint_at(utils::sfml_to_vec2d(mouse_pos));
	jh.set_target(jad.dble_joint, jad.type);
}

void app::remove(const body* b)
{
	if (b == nullptr) return;
	_bodies.erase(std::remove(_bodies.begin(), _bodies.end(), b));
	delete b;
}

void app::remove(const dble_joint* j)
{
	if (j == nullptr) return;
	_dble_joints.erase(std::remove(_dble_joints.begin(), _dble_joints.end(), j));
	delete j;
}

void app::remove_all()
{
	std::vector<body*> _bodies_cpy(_bodies);
	std::vector<dble_joint*> _dble_joints_cpy(_dble_joints);
	for (auto& i : _bodies_cpy) remove(i);
	for (auto& i : _dble_joints_cpy) remove(i);
}

void app::start_simulation()
{
	_mode = app_mode::simulation;
	save_json("C:\\Users\\chedo\\OneDrive\\Pulpit\\test.json");

	/* Load all application elements into tho the scene object */
	_scene.remove_all();
	for (body* b : _bodies) {
		_scene.add(b);
	}
	for (dble_joint* j : _dble_joints) {
		if (j->body_a() && j->body_b()) {
			_scene.add(j->joint());
		}
	}

	bh.set_target(nullptr);
	jh.set_target(nullptr);
	jc_ui.set_target(nullptr);
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

	for (const body* body_it : _bodies) {
		json_obj["bodies"].push_back(json_utils::serialize(*body_it));
	}
	for (dble_joint* joint_it : _dble_joints) {
		json_obj["joints"].push_back(json_utils::serialize(joint_it));
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
	for (auto* j : _dble_joints) {
		delete j;
	}
	_bodies.clear();
	_dble_joints.clear();

	for (const auto& i : json_obj["bodies"]) {
		// iteracja po kolejnych obiektach
		body* b = new body(json_utils::deserialize_body(i));
		b->update_shape();
		_bodies.push_back(b);
		_current_id = std::max(_current_id, b->id());
	}
	for (const auto& i : json_obj["joints"]) {
		// iteracja po kolejnych przegubach
		dble_joint* j = json_utils::deserialize_joint(i, _bodies);
		_dble_joints.push_back(j);
	}
	ifs.close();
}

void app::set_bodies_display_mode(body::display_mode inside, body::display_mode outline)
{
	for (auto& b : _bodies) {
		b->set_body_display_mode(inside);
		b->set_edges_display_mode(outline);
	}
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
	for (dble_joint* dj : _dble_joints) {
		ds2::joint* i = dj->joint();
		ds2::joint_type jt = i->type();
		double dist_a = (scene_pos - i->global_a()).len();
		double dist_b = (scene_pos - i->global_b()).len();

		constexpr double snap_r = 
			dble_joint_conf::joint_radius + dble_joint_conf::joint_outline_thickness;

		if (jt == ds2::joint_type::hinge || jt == ds2::joint_type::motor) {
			if (dist_a < snap_r) return joint_at_data{ dj, joint_handler_mode::both };
		}
		else {
			if (dist_a < snap_r) return joint_at_data{ dj, joint_handler_mode::a };
			if (dist_b < snap_r) return joint_at_data{ dj, joint_handler_mode::b };
		}
	}
	return joint_at_data{ nullptr, joint_handler_mode::a };
}


