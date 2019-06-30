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
    class AppState;
	class GCodeViewParser
	{
	public:
		GCodeViewParser(AppState*);
		~GCodeViewParser();

		vec3 GetMinimumExtremes() const ;
		vec3 GetMaximumExtremes() const;
		double GetMinLength() const;
		vec2 GetResolution() const;
		vector<LineSegment>& GetLineSegmentHandlesList();
		vector<vector<int>>& GetLinesIndexes();
		void SetLinesFromParser(GCodeParser& gp, double arcPrecision = 1.0, bool arcDegreeMode=true);

		void ClearState();

    protected:
        void TestExtremes(const vec3 &p3d);
		void TestExtremes(double x, double y, double z);
		void TestLength(const vec3 &start, const vec3 &end);

	private:
        AppState* mAppState;
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
