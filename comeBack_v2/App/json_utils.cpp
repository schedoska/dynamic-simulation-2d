#include "json_utils.h"

nlohmann::json json_utils::serialize(const body& b)
{
	nlohmann::json json_obj;

	json_obj["id"] = b.id();
	json_obj["pos"] = serialize(b.pos());
	json_obj["vel"] = serialize(b.vel());
	json_obj["inertia"] = b.inertia();
	json_obj["rot"] = b.rot();
	json_obj["rot_vel"] = b.rot_vel();
	json_obj["mass"] = b.mass();
	json_obj["layer min"] = b.layer_min();
	json_obj["layer max"] = b.layer_max();
	json_obj["shape"] = serialize(b.shape());

	json_obj["name"] = b.name();
	json_obj["color"] = serialize(b.fill_color());

	return json_obj;
}

nlohmann::json json_utils::serialize(const vl::vec2d& v)
{
	return nlohmann::json{ {"x", v[0]}, {"y", v[1]} };
}

nlohmann::json json_utils::serialize(const ds2::shape_group& sg)
{
	nlohmann::json json_sg;

	for (const auto& circ : sg.circles()) {
		nlohmann::json json_circ;
		json_circ["loc_pos"] = serialize(circ.loc_pos());
		json_circ["radius"] = circ.radius();
		json_sg["circles"].push_back(json_circ);
	}
	for (const auto& conv : sg.convexes()) {
		nlohmann::json json_conv;
		for (const auto& vx : conv.vertices()) {
			json_conv["vertices"].push_back(serialize(vx));
		}
		json_sg["convexes"].push_back(json_conv);
	}
	return json_sg;
}

nlohmann::json json_utils::serialize(const sf::Color& c)
{
	return nlohmann::json{ {"r", c.r}, {"g", c.g}, {"b", c.b}, {"a", c.a} };
}

nlohmann::json json_utils::serialize(dble_joint* h)
{
	nlohmann::json json_joint;
	ds2::joint *j = h->joint();

	json_joint["loc_a"] = serialize(j->loc_a());
	json_joint["loc_b"] = serialize(j->loc_b());

	json_joint["body_a"] = h->body_a() ? h->body_a()->id() : -1;
	json_joint["body_b"] = h->body_b() ? h->body_b()->id() : -1;

	switch (h->joint()->type())
	{
	case ds2::joint_type::spring:
		serialize(json_joint, dynamic_cast<dble_spring*>(h));
		break;
	case ds2::joint_type::hinge:
		serialize(json_joint, dynamic_cast<dble_hinge*>(h));
		break;
	case ds2::joint_type::motor:
		serialize(json_joint, dynamic_cast<dble_motor*>(h));
		break;
	default:
		return json_joint;
	};
	return json_joint;
}

void json_utils::serialize(nlohmann::json& j, dble_hinge* h)
{
	j["type"] = ds2::joint_type::hinge;
	j["stiffness"] = h->hinge_joint()->stiffness();
	j["friction"] = h->hinge_joint()->friction();
}

void json_utils::serialize(nlohmann::json& j, dble_spring* h)
{
	j["type"] = ds2::joint_type::spring;
	j["length"] = h->spring_joint()->length();
	j["strength"] = h->spring_joint()->strength();
	j["damping"] = h->spring_joint()->damping();
}

void json_utils::serialize(nlohmann::json& j, dble_motor* h)
{
	j["type"] = ds2::joint_type::motor;
	j["ang_vel"] = h->motor_joint()->ang_vel();
	j["torque"] = h->motor_joint()->torque();
	j["stiffness"] = h->motor_joint()->stiffness();
	j["friction"] = h->motor_joint()->friction();
}

dble_joint* json_utils::deserialize_joint(const nlohmann::json& json_j, std::vector<body*>& bodies)
{
	switch (json_j["type"].get<ds2::joint_type>()) {
	case ds2::joint_type::spring:
		return deserialize_spring(json_j, bodies);
	case ds2::joint_type::hinge:
		return deserialize_hinge(json_j, bodies);
	case ds2::joint_type::motor:
		return deserialize_motor(json_j, bodies);
	}
}

dble_hinge* json_utils::deserialize_hinge(const nlohmann::json& json_j, std::vector<body*>& bodies)
{
	vl::vec2d loc_a = deserialize_vec2d(json_j["loc_a"]);
	vl::vec2d loc_b = deserialize_vec2d(json_j["loc_b"]);
	int id_a = json_j["body_a"].get<int>();
	int id_b = json_j["body_b"].get<int>();
	body* a = id_a == -1 ? nullptr : body_of_id(id_a, bodies);
	body* b = id_b == -1 ? nullptr : body_of_id(id_b, bodies);

	dble_hinge* hinge = new dble_hinge(a, b, loc_a, loc_b);
	hinge->hinge_joint()->set_stiffness(json_j["stiffness"].get<double>());
	hinge->hinge_joint()->set_friction(json_j["friction"].get<double>());
	return hinge;
}

dble_spring* json_utils::deserialize_spring(const nlohmann::json& json_j, std::vector<body*>& bodies)
{
	vl::vec2d loc_a = deserialize_vec2d(json_j["loc_a"]);
	vl::vec2d loc_b = deserialize_vec2d(json_j["loc_b"]);
	int id_a = json_j["body_a"].get<int>();
	int id_b = json_j["body_b"].get<int>();
	body* a = id_a == -1 ? nullptr : body_of_id(id_a, bodies);
	body* b = id_b == -1 ? nullptr : body_of_id(id_b, bodies);

	dble_spring* spring = new dble_spring(a, b, loc_a, loc_b);
	spring->spring_joint()->set_length(json_j["length"].get<double>());
	spring->spring_joint()->set_damping(json_j["damping"].get<double>());
	spring->spring_joint()->set_strength(json_j["strength"].get<double>());
	return spring;
}

dble_motor* json_utils::deserialize_motor(const nlohmann::json& json_j, std::vector<body*>& bodies)
{
	vl::vec2d loc_a = deserialize_vec2d(json_j["loc_a"]);
	vl::vec2d loc_b = deserialize_vec2d(json_j["loc_b"]);
	int id_a = json_j["body_a"].get<int>();
	int id_b = json_j["body_b"].get<int>();
	body* a = id_a == -1 ? nullptr : body_of_id(id_a, bodies);
	body* b = id_b == -1 ? nullptr : body_of_id(id_b, bodies);

	dble_motor* motor = new dble_motor(a, b, loc_a, loc_b);
	motor->motor_joint()->set_ang_vel(json_j["ang_vel"].get<double>());
	motor->motor_joint()->set_torque(json_j["torque"].get<double>());
	motor->motor_joint()->set_stiffness(json_j["stiffness"].get<double>());
	motor->motor_joint()->set_friction(json_j["friction"].get<double>());
	return motor;
}

vl::vec2d json_utils::deserialize_vec2d(const nlohmann::json& json_v)
{
	return vl::vec2d(json_v["x"].get<double>(), json_v["y"].get<double>());
}

body json_utils::deserialize_body(const nlohmann::json& json_b)
{
	body b(json_b["id"].get<int>());
	b.set_name(json_b["name"].get<std::string>());

	b.pos() = deserialize_vec2d(json_b["pos"]);
	b.vel() = deserialize_vec2d(json_b["vel"]);
	b.inertia() = json_b["inertia"].get<double>();
	b.rot() = json_b["rot"].get<double>();
	b.rot_vel() = json_b["rot_vel"].get<double>();
	b.set_mass(json_b["mass"].get<double>());
	b.layer_min() = json_b["layer min"].get<int>();
	b.layer_max() = json_b["layer max"].get<int>();
	b.shape() = deserialize_shape_group(json_b["shape"]);
	b.set_fill_color(deserialize_color(json_b["color"]));

	return b;
}

ds2::shape_group json_utils::deserialize_shape_group(const nlohmann::json& json_b)
{
	ds2::shape_group sg;
	if (json_b.contains("convexes")) {
		for (const auto& convex_it : json_b["convexes"]) {
			ds2::convex_shape cs;
			for (const auto& vertex_it : convex_it["vertices"]) {
				cs.add(deserialize_vec2d(vertex_it));
			}
			sg.add(cs);
		}
	}
	if (json_b.contains("circles")) {
		for (const auto& circle_it : json_b["circles"]) {
			ds2::circle_shape cs;
				cs.set_loc_pos(deserialize_vec2d(circle_it["loc_pos"]));
				cs.set_radius(circle_it["radius"].get<double>());
				sg.add(cs);
		}
	}
	return sg;
}

sf::Color json_utils::deserialize_color(const nlohmann::json& json_c)
{
	return sf::Color(
		json_c["r"].get<sf::Uint8>(), 
		json_c["g"].get<sf::Uint8>(),
		json_c["b"].get<sf::Uint8>(),
		json_c["a"].get<sf::Uint8>());
}

body* json_utils::body_of_id(const unsigned int& id, std::vector<body*>& bodies)
{
	for (body* body_it : bodies) {
		if (body_it->id() == id) return body_it;
	}
}
