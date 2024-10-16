#pragma once

#include "body.h"
#include "dble_joint.h"
#include <nlohmann/json.hpp>

namespace json_utils
{
	nlohmann::json serialize(const body& b);
	nlohmann::json serialize(const vl::vec2d& v);
	nlohmann::json serialize(const ds2::shape_group& sg);
	nlohmann::json serialize(const sf::Color& c);

	nlohmann::json serialize(const dble_hinge& h);

	vl::vec2d deserialize_vec2d(const nlohmann::json& json_v);
	body deserialize_body(const nlohmann::json& json_b);
	ds2::shape_group deserialize_shape_group(const nlohmann::json& json_b);
	sf::Color deserialize_color(const nlohmann::json& json_v);
};

