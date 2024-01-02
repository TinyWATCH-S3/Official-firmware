#pragma once

#include <Arduino.h>

#include "json.h"

// Arduino string to and from json
static void to_json(nlohmann::json &j, const String &value)
{
	j = value.c_str();
}
static void from_json(const nlohmann::json &j, String &s)
{
	if (!j.is_string())
	{
		throw nlohmann::json::type_error::create(302, "type must be string, but is " + std::string(j.type_name()), j);
	}
	s = j.get_ptr<const std::string *>()->c_str();
}
