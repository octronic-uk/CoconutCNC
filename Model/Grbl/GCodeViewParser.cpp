// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeViewParse.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GCodeViewParser.h"

#include "GCodeFileModel.h"
#include "../../AppState.h"
#include "../../Common/Logger.h"
#include "../../Common/Util.h"
#include "../../Widgets/GL/GCodeDrawer.h"

namespace Coconut
{
	GCodeViewParser::GCodeViewParser(AppState* state)
        : mAppState(state)
	{
		mAbsoluteMode = true;
		mAbsoluteIJK = false;
		mCurrentLine = 0;
		mDebug = true;

		mMin = vec3(NAN, NAN, NAN);
		mMax = vec3(NAN, NAN, NAN);

		mMinLength = NAN;
	}

	GCodeViewParser::~GCodeViewParser()
	{
	}

	vec3 GCodeViewParser::GetMinimumExtremes() const
	{
		return mMin;
	}

	vec3 GCodeViewParser::GetMaximumExtremes() const
	{
		return mMax;
	}

	void GCodeViewParser::TestExtremes(const vec3 &p3d)
	{
		TestExtremes(p3d.x, p3d.y, p3d.z);
	}

	void GCodeViewParser::TestExtremes(double x, double y, double z)
	{
		mMin.x = Util::nMin(mMin.x, x);
		mMin.y = Util::nMin(mMin.y, y);
		mMin.z = Util::nMin(mMin.z, z);

		mMax.x = Util::nMax(mMax.x, x);
		mMax.y = Util::nMax(mMax.y, y);
		mMax.z = Util::nMax(mMax.z, z);
	}

	void GCodeViewParser::TestLength(const vec3 &start, const vec3 &end)
	{
		double length = (start - end).length();
		if (!isnan(length) && length != 0)
		{
			mMinLength = isnan(mMinLength) ?
			length :
			min<double>(mMinLength, length);
		}
	}

	vector<LineSegment>& GCodeViewParser::GetLineSegmentHandlesList()
	{
		return mLines;
	}

	void GCodeViewParser::ClearState()
	{
        mAbsoluteMode = true;
		mAbsoluteIJK = false;
		mCurrentLine = 0;
		mDebug = true;

		mMin = vec3(NAN, NAN, NAN);
		mMax = vec3(NAN, NAN, NAN);

		mMinLength = NAN;

		mLines.clear();
		mLineIndexes.clear();
		mCurrentLine = 0;
	}

	double GCodeViewParser::GetMinLength() const
	{
		return mMinLength;
	}

	vec2 GCodeViewParser::GetResolution() const
	{
		return vec2
		(
			((mMax.x - mMin.x) / mMinLength) + 1,
			((mMax.y - mMin.y) / mMinLength) + 1
		);
	}

	void GCodeViewParser::SetLinesFromParser
	(GCodeParser& gp, double arcPrecision, bool arcDegreeMode)
	{
        info("GCodeViewParser: {}",__FUNCTION__);
		// Remove old
		mLines.clear();

		vector <PointSegment> psl = gp.GetPointSegmentList();
		info("GCodeViewParser: Point Segments {}", psl.size());

		// For a line segment list ALL arcs must be converted to lines.
		double minArcLength = 0.1;
		//double length;

		vec3 start(NAN);
        vec3 end(NAN);

		// Prepare segments indexes
		mLineIndexes.resize(psl.size());

		int lineIndex = 0;
		for (PointSegment& segment : psl)
		{
			PointSegment& ps = segment;
			bool isMetric = ps.IsMetric();
			ps.ConvertToMetric();

			end = ps.GetPoint();

			// start is null for the first iteration.
			if (start != vec3(NAN))
			{
				// Expand arc for graphics.
				if (ps.IsArc())
				{
					vector<vec3> points =
						GCodeParser::GeneratePointsAlongArcBDring
						(
							ps.Plane(),
							start, end, ps.Center(),
							ps.IsClockwise(), ps.GetRadius(),
							minArcLength, arcPrecision, arcDegreeMode
						);
					// Create line segments from points.
					if (points.size() > 0)
					{
						vec3 startPoint = vec3(NAN);
						for (vec3 nextPoint : points)
						{
							if (nextPoint == startPoint) continue;
							auto ls = LineSegment(startPoint, nextPoint, lineIndex);
							ls.setIsArc(ps.IsArc());
							ls.setIsClockwise(ps.IsClockwise());
							ls.setPlane(ps.Plane());
							ls.setIsRapidMovement(ps.IsRapidMovement());
							ls.setIsZMovement(ps.IsZMovement());
							ls.setIsMetric(isMetric);
							ls.setIsAbsolute(ps.IsAbsolute());
							ls.setSpeed(ps.GetSpeed());
							ls.setSpindleSpeed(ps.GetSpindleSpeed());
							ls.setDwell(ps.GetDwell());

							TestExtremes(nextPoint);

							mLines.push_back(ls);

							mLineIndexes[ps.GetLineNumber()].push_back(mLines.size() - 1);

							startPoint = nextPoint;
						}
						lineIndex++;
					}
				// Line
				}
                else
                {
					auto ls = LineSegment(start, end, lineIndex++);
					ls.setIsArc(ps.IsArc());
					ls.setIsRapidMovement(ps.IsRapidMovement());
					ls.setIsZMovement(ps.IsZMovement());
					ls.setIsMetric(isMetric);
					ls.setIsAbsolute(ps.IsAbsolute());
					ls.setSpeed(ps.GetSpeed());
					ls.setSpindleSpeed(ps.GetSpindleSpeed());
					ls.setDwell(ps.GetDwell());

					TestExtremes(end);
					TestLength(start, end);

					mLines.push_back(ls);
					mLineIndexes[ps.GetLineNumber()].push_back(mLines.size() - 1);
				}
			}
			start = end;
		}
        mAppState->GetGCodeDrawer().ReadFromFileModel();
	}

	vector<vector<int>>& GCodeViewParser::GetLinesIndexes()
	{
		return mLineIndexes;
	}
}
