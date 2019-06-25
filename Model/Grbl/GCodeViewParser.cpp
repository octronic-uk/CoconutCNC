// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeViewParse.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GCodeViewParser.h"
#include "../../Common/Logger.h"
#include "../../Common/Util.h"

namespace Coconut
{
	GCodeViewParser::GCodeViewParser()
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

	vec3 GCodeViewParser::getMinimumExtremes() const
	{
		return mMin;
	}

	vec3 GCodeViewParser::getMaximumExtremes() const
	{
		return mMax;
	}

	void GCodeViewParser::testExtremes(const vec3 &p3d)
	{
		testExtremes(p3d.x, p3d.y, p3d.z);
	}

	void GCodeViewParser::testExtremes(double x, double y, double z)
	{
		mMin.x = Util::nMin(mMin.x, x);
		mMin.y = Util::nMin(mMin.y, y);
		mMin.z = Util::nMin(mMin.z, z);

		mMax.x = Util::nMax(mMax.x, x);
		mMax.y = Util::nMax(mMax.y, y);
		mMax.z = Util::nMax(mMax.z, z);
	}

	void GCodeViewParser::testLength(const vec3 &start, const vec3 &end)
	{
		double length = (start - end).length();
		if (!isnan(length) && length != 0)
		{
			mMinLength = isnan(mMinLength) ?
			length :
			min<double>(mMinLength, length);
		}
	}

	vector<LineSegment*> GCodeViewParser::getLineSegmentHandlesList() const
	{
		vector<LineSegment*> handles;
		for (auto ls : mLines) handles.push_back(&ls);
		return handles;
	}

	void GCodeViewParser::reset()
	{
		mLines.clear();
		mLineIndexes.clear();
		mCurrentLine = 0;
		mMin = vec3(NAN, NAN, NAN);
		mMax = vec3(NAN, NAN, NAN);
		mMinLength = NAN;
	}

	double GCodeViewParser::getMinLength() const
	{
		return mMinLength;
	}

	vec2 GCodeViewParser::getResolution() const
	{
		return vec2
		(
			((mMax.x - mMin.x) / mMinLength) + 1,
			((mMax.y - mMin.y) / mMinLength) + 1
		);
	}

	void GCodeViewParser::setLinesFromParser
	(GCodeParser *gp, double arcPrecision, bool arcDegreeMode)
	{
		// Remove old
		mLines.clear();

		vector <PointSegment*> psl = gp->getPointSegmentHandlesList();
		debug("GCodeViewParser: Point Segments {}", psl.size());

		// For a line segment list ALL arcs must be converted to lines.
		double minArcLength = 0.1;
		//double length;

		vec3 *start, *end;
		start = nullptr;
		end = nullptr;

		// Prepare segments indexes
		mLineIndexes.resize(psl.size());

		int lineIndex = 0;
		for (PointSegment *segment : psl)
		{
			PointSegment *ps = segment;
			bool isMetric = ps->isMetric();
			ps->convertToMetric();

			end = ps->getPoint();

			// start is null for the first iteration.
			if (start != nullptr)
			{
				// Expand arc for graphics.
				if (ps->isArc())
				{
					vector<vec3> points =
						GCodeParser::generatePointsAlongArcBDring
						(
							ps->plane(),
							*start, *end, ps->center(),
							ps->isClockwise(), ps->getRadius(),
							minArcLength, arcPrecision, arcDegreeMode
						);
					// Create line segments from points.
					if (points.size() > 0)
					{
						vec3 startPoint = *start;
						for (vec3 nextPoint : points)
						{
							if (nextPoint == startPoint) continue;
							auto ls = LineSegment(startPoint, nextPoint, lineIndex);
							ls.setIsArc(ps->isArc());
							ls.setIsClockwise(ps->isClockwise());
							ls.setPlane(ps->plane());
							ls.setIsRapidMovement(ps->isRapidMovement());
							ls.setIsZMovement(ps->isZMovement());
							ls.setIsMetric(isMetric);
							ls.setIsAbsolute(ps->isAbsolute());
							ls.setSpeed(ps->getSpeed());
							ls.setSpindleSpeed(ps->getSpindleSpeed());
							ls.setDwell(ps->getDwell());

							testExtremes(nextPoint);

							mLines.push_back(ls);

							mLineIndexes[ps->getLineNumber()].push_back(mLines.size() - 1);

							startPoint = nextPoint;
						}
						lineIndex++;
					}
				// Line
				}
                else
                {
					auto ls = LineSegment(*start, *end, lineIndex++);
					ls.setIsArc(ps->isArc());
					ls.setIsRapidMovement(ps->isRapidMovement());
					ls.setIsZMovement(ps->isZMovement());
					ls.setIsMetric(isMetric);
					ls.setIsAbsolute(ps->isAbsolute());
					ls.setSpeed(ps->getSpeed());
					ls.setSpindleSpeed(ps->getSpindleSpeed());
					ls.setDwell(ps->getDwell());

					testExtremes(*end);
					testLength(*start, *end);

					mLines.push_back(ls);
					mLineIndexes[ps->getLineNumber()].push_back(mLines.size() - 1);
				}
			}
			start = end;
		}
	}

	vector<vector<int>> GCodeViewParser::getLinesIndexes() const
	{
		return mLineIndexes;
	}
}
