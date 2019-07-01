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
#include "JsonSerialization.h"

namespace Coconut
{
    JsonSerialization::JsonSerialization() {}

    vec2 JsonSerialization::JsonToVec2(const json& js)
    {
        vec2 v(0);
        if (js.is_array() && js.size() == 2)
        {
       		v.x = js[0];
       		v.y = js[1];
        }
        return v;
    }

    json JsonSerialization::Vec2ToJson(const vec2& v)
    {
		json j = json::array();
        j.push_back(v.x);
        j.push_back(v.y);
        return j;
    }

    vec3 JsonSerialization::JsonToVec3(const json& js)
    {
        vec3 v(0.0f);
        if (js.is_array() && js.size() == 3)
        {
       		v.x = js[0];
       		v.y = js[1];
            v.z = js[2];
        }
        return v;
    }

    json JsonSerialization::Vec3ToJson(const vec3& v)
    {
		json j = json::array();
        j.push_back(v.x);
        j.push_back(v.y);
        j.push_back(v.z);
        return j;
    }

    vec4 JsonSerialization::JsonToVec4(const json& js)
    {
        vec4 v(0);
        if (js.is_array() && js.size() == 4)
        {
       		v.x = js[0];
       		v.y = js[1];
            v.z = js[2];
            v.w = js[3];
        }
        return v;
    }

    json JsonSerialization::Vec4ToJson(const vec4& v)
    {
		json j = json::array();
        j.push_back(v.x);
        j.push_back(v.y);
        j.push_back(v.z);
        j.push_back(v.w);
        return j;
    }
}
