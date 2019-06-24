// This file is a part of "CoconutCNC" application.
// This file was originally ported from "LineSegment.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "LineSegment.h"

namespace Coconut
{
	LineSegment::LineSegment()
	{
		mToolhead = 0;
		mIsZMovement = false;
		mIsArc = false;
		mIsRapidMovement = false;
		mDrawn = false;
		mIsMetric = true;
		mIsAbsolute = true;
		mIsHightlight = false;
		mVertexIndex = -1;
	}

	LineSegment::LineSegment(const vec3 &a, const vec3 &b, int num)
		: LineSegment()
	{
		mFirst = a;
		mSecond = b;
		mLineNumber = num;
	}

	LineSegment::LineSegment(const LineSegment& initial)
	{
		mToolhead = initial.getToolhead();
		mIsZMovement = initial.isZMovement();
		mIsArc = initial.isArc();
		mIsRapidMovement = initial.isRapidMovement();
		mDrawn = initial.drawn();
		mFirst = initial.getStart();
		mSecond = initial.getEnd();
		mLineNumber = initial.getLineNumber();
		mSpeed = initial.getSpeed();
		mIsMetric = initial.isMetric();
		mIsAbsolute = initial.isAbsolute();
		mIsHightlight = initial.isHightlight();
		mVertexIndex = initial.vertexIndex();
	}

	LineSegment::~LineSegment()
	{

	}

	int LineSegment::getLineNumber() const
	{
		return mLineNumber;
	}

	vector<vec3> LineSegment::getPointArray() const
	{
		vector<vec3> pointarr;
		pointarr.push_back(mFirst);
		pointarr.push_back(mSecond);
		return pointarr;
	}

	vector<double> LineSegment::getPoints() const
	{
		vector<double> points;
		points.push_back(mFirst.x);
		points.push_back(mFirst.y);
		points.push_back(mFirst.z);
		points.push_back(mSecond.x);
		points.push_back(mSecond.y);
		points.push_back(mSecond.z);
		return points;
	}

	vec3 LineSegment::getStart() const
	{
		return mFirst;
	}

	void LineSegment::setStart(const vec3& vector)
	{
		mFirst = vector;
	}

	vec3 LineSegment::getEnd() const
	{
		return mSecond;
	}

	void LineSegment::setEnd(const vec3& vector)
	{
		mSecond = vector;
	}

	void LineSegment::setToolHead(int head)
	{
		mToolhead = head;
	}

	int LineSegment::getToolhead() const
	{
		return mToolhead;
	}

	void LineSegment::setSpeed(double s)
	{
		mSpeed = s;
	}

	double LineSegment::getSpeed() const
	{
		return mSpeed;
	}

	void LineSegment::setIsZMovement(bool isZ)
	{
		mIsZMovement = isZ;
	}

	bool LineSegment::isZMovement() const
	{
		return mIsZMovement;
	}

	void LineSegment::setIsArc(bool isA)
	{
		mIsArc = isA;
	}

	bool LineSegment::isArc()  const
	{
		return mIsArc;
	}

	void LineSegment::setIsRapidMovement(bool isF)
	{
		mIsRapidMovement = isF;
	}

	bool LineSegment::isRapidMovement() const
	{
		return mIsRapidMovement;
	}

	bool LineSegment::contains(const vec3 &point) const
	{
		double delta;
		vec3 line = getEnd() - getStart();
		vec3 pt = point - getStart();

		delta = (line - pt).length() - (line.length() - pt.length());

		return delta < 0.01;
	}

	bool LineSegment::drawn() const
	{
		return mDrawn;
	}

	void LineSegment::setDrawn(bool drawn)
	{
		mDrawn = drawn;
	}
	bool LineSegment::isMetric() const
	{
		return mIsMetric;
	}

	void LineSegment::setIsMetric(bool isMetric)
	{
		mIsMetric = isMetric;
	}
	bool LineSegment::isAbsolute() const
	{
		return mIsAbsolute;
	}

	void LineSegment::setIsAbsolute(bool isAbsolute)
	{
		mIsAbsolute = isAbsolute;
	}
	bool LineSegment::isHightlight() const
	{
		return mIsHightlight;
	}

	void LineSegment::setIsHightlight(bool isHightlight)
	{
		mIsHightlight = isHightlight;
	}

	int LineSegment::vertexIndex() const
	{
		return mVertexIndex;
	}

	void LineSegment::setVertexIndex(int vertexIndex)
	{
		mVertexIndex = vertexIndex;
	}

	double LineSegment::getSpindleSpeed() const
	{
		return mSpindleSpeed;
	}

	void LineSegment::setSpindleSpeed(double spindleSpeed)
	{
		mSpindleSpeed = spindleSpeed;
	}

	double LineSegment::getDwell() const
	{
		return mDwell;
	}

	void LineSegment::setDwell(double dwell)
	{
		mDwell = dwell;
	}

	bool LineSegment::isClockwise() const
	{
		return mIsClockwise;
	}

	void LineSegment::setIsClockwise(bool isClockwise)
	{
		mIsClockwise = isClockwise;
	}

	PointSegment::planes LineSegment::plane() const
	{
		return mPlane;
	}

	void LineSegment::setPlane(const PointSegment::planes &plane)
	{
		mPlane = plane;
	}

}
