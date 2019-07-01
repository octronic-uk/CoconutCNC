/*
 * Copyright 2018 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */
#pragma once

#include <json.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using nlohmann::json;
using glm::vec2;
using glm::vec3;
using glm::vec4;

namespace Coconut
{
	class JsonSerialization
	{
	public:
		JsonSerialization();
        virtual json ToJson() = 0;
        virtual bool FromJson(const json& j) = 0;

        vec2 JsonToVec2(const json& js);
        json Vec2ToJson(const vec2& v);

        vec3 JsonToVec3(const json& js);
        json Vec3ToJson(const vec3& v);

        vec4 JsonToVec4(const json& js);
        json Vec4ToJson(const vec4& v);

	};
}

