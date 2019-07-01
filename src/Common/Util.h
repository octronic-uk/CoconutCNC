// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <algorithm>
#include <locale>
#include <string>
#include <cctype>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using std::isnan;
using std::nan;
using std::toupper;
using std::string;
using std::min;
using std::max;
using std::transform;

using glm::vec4;
using glm::vec3;

namespace Coconut
{
	class Util
	{
	public:
		static inline float nMin(float  v1, float v2)
		{
			if (!isnan(v1) && !isnan(v2)) return min<float>(v1, v2);
			else if (!isnan(v1)) return v1;
			else if (!isnan(v2)) return v2;
			else return NAN;
		}

		static inline float nMax(float v1, float v2)
		{
			if (!isnan(v1) && !isnan(v2)) return max<float>(v1, v2);
			else if (!isnan(v1)) return v1;
			else if (!isnan(v2)) return v2;
			else return NAN;
		}

		static inline float toMetric(float value)
		{
			return value * 25.4f;
		}

		static inline bool compareCoordinates(vec3 machine, float x, float y, float z)
		{
			return machine.x == x && machine.y == y && machine.z == z;
		}

		static string to_upper(const string& str)
		{
            string s = str;
			transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
            {
				return std::toupper(c);
			});
			return s;
		}

		// trim from start (in place)
		static inline void ltrim(std::string &s)
        {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
            {
				return !std::isspace(ch);
			}));
		}

		// trim from end (in place)
		static inline void rtrim(std::string &s)
        {
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
            {
				return !std::isspace(ch);
			}).base(), s.end());
		}

		// trim from both ends (in place)
		static inline void trim(std::string &s)
        {
			ltrim(s);
			rtrim(s);
		}

		// trim from start (copying)
		static inline std::string ltrim_copy(std::string s)
        {
			ltrim(s);
			return s;
		}

		// trim from end (copying)
		static inline std::string rtrim_copy(std::string s)
        {
			rtrim(s);
			return s;
		}

		// trim from both ends (copying)
		static inline std::string trim_copy(std::string s)
        {
			trim(s);
			return s;
		}
	};
}
