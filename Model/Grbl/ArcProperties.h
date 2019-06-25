// This file is a part of "CoconutCNC" application.
// This file was originally ported from "ArcProperties.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <glm/vec3.hpp>

using glm::vec3;

namespace Coconut
{

	class ArcProperties
	{
	public:
		explicit ArcProperties();
		ArcProperties(const ArcProperties& other);
		bool isClockwise;
		double radius;
		vec3 center;
	};

}
