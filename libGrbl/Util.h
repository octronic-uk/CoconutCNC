// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <string>
#include <cctype>
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
		static double nMin(double v1, double v2)
		{
			if (!isnan(v1) && !isnan(v2)) return min<double>(v1, v2);
			else if (!isnan(v1)) return v1;
			else if (!isnan(v2)) return v2;
			else return NAN;
		}

		static double nMax(double v1, double v2)
		{
			if (!isnan(v1) && !isnan(v2)) return max<double>(v1, v2);
			else if (!isnan(v1)) return v1;
			else if (!isnan(v2)) return v2;
			else return NAN;
		}

		static double toMetric(double value)
		{
			return value * 25.4;
		}

		static bool compareCoordinates(vec3 machine, double x, double y, double z)
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
	};
}
