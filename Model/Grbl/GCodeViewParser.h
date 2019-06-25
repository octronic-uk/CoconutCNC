// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeViewParse.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include "LineSegment.h"
#include "GCodeParser.h"
#include "Util.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using glm::vec2;
using glm::vec3;

namespace Coconut
{
	class GCodeViewParser
	{
	public:
		explicit GCodeViewParser();
		~GCodeViewParser();

		vec3 getMinimumExtremes() const ;
		vec3 getMaximumExtremes() const;
		double getMinLength() const;
		vec2 getResolution() const;
		vector<LineSegment*> getLineSegmentHandlesList() const;
		vector<vector<int>> getLinesIndexes() const;
		void setLinesFromParser(GCodeParser* gp, double arcPrecision, bool arcDegreeMode);

		void reset();

    protected:
        void testExtremes(const vec3 &p3d);
		void testExtremes(double x, double y, double z);
		void testLength(const vec3 &start, const vec3 &end);

	private:
		bool mAbsoluteMode;
		bool mAbsoluteIJK;

		// Parsed object
		vec3 mMin, mMax;
		double mMinLength;
		vector<LineSegment> mLines;
		vector<vector<int>> mLineIndexes;

		// Parsing state.
		vec3 mLastPoint;
		int mCurrentLine; // for assigning line numbers to segments.

		// Debug
		bool mDebug;

	};
}
