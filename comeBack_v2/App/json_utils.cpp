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
	json_obj["color"] = serialize(b.color());

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

nlohmann::json json_utils::serialize(const dble_hinge& h)
{
	nlohmann::json json_obj; 
	json_obj["loc_a"] = serialize(h.hinge_joint()->loc_a());
	json_obj["loc_b"] = serialize(h.hinge_joint()->loc_b());
	
	return nlohmann::json();
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
	b.set_color(deserialize_color(json_b["color"]));

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
