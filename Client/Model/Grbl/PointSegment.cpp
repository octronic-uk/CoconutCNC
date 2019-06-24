// This file is a part of "CoconutCNC" application.
// This file was originally ported from "PointSegment.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "PointSegment.h"
#include "../../Logger.h"

namespace Coconut
{
	PointSegment::PointSegment(GCodeCommand* parent) :
        mParent(parent),
		mToolhead(0),
		mIsMetric(true),
		mIsAbsolute(true),
		mIsZMovement(false),
		mIsArc(false),
		mIsRapidMovement(false),
		mLineNumber(-1),
		mSpeed(0),
		mSpindleSpeed(0),
		mDwell(0),
		mPlane(XY),
		mPoint(vec3(0.0))
	{
	//    qDebug() << "PointSegment: Constructor";
	}

	PointSegment::PointSegment(const PointSegment &ps)
		:
		  mToolhead(ps.getToolhead()),
		  mSpeed(ps.getSpeed()),
		  mPoint(ps.mPoint),
		  mIsMetric(ps.isMetric()),
		  mIsZMovement(ps.isZMovement()),
		  mIsArc(ps.isArc()),
		  mIsRapidMovement(ps.isRapidMovement()),
		  mIsAbsolute(ps.isAbsolute()),
		  mLineNumber(ps.mLineNumber)
	{
	    debug("PointSegment: Copy Constructor, isZMovement {}" ,mIsZMovement);
		if (isArc())
		{
			//qDebug() << "PointSegment: isArc";
			setArcCenter(ps.center());
			setRadius(ps.getRadius());
			setIsClockwise(ps.isClockwise());
			mPlane = ps.plane();
		}
	}

	PointSegment::PointSegment(GCodeCommand* parent, const vec3 &b, int num)
		: PointSegment(parent)

	{
	    debug("PointSegment: vec3 Constructor, {} {} {} / {}" , b.x, b.y, b.z,  num);
		mPoint = vec3(b);
		mLineNumber = num;
	}

	PointSegment::PointSegment
	(
		GCodeCommand* parent,
		const vec3 &point,
		int num,
		const vec3 &center,
		double radius,
		bool clockwise
	) : PointSegment(parent, point, num)
	{
		mIsArc = true;
		mArcProperties.center = vec3(center);
		mArcProperties.radius = radius;
		mArcProperties.isClockwise = clockwise;
	}

	PointSegment::~PointSegment()
	{
	}

	vec3* PointSegment::getPoint()
	{
		return &mPoint;
	}

	vector<double> PointSegment::points() const
	{
		vector<double> points;
		points.push_back(mPoint.x);
		points.push_back(mPoint.y);
		return points;
	}

	void PointSegment::setToolHead(int head)
	{
		mToolhead = head;
	}

	int PointSegment::getToolhead() const
	{
		return mToolhead;
	}

	void PointSegment::setLineNumber(int num)
	{
		mLineNumber = num;
	}

	int PointSegment::getLineNumber() const
	{
		return mLineNumber;
	}

	void PointSegment::setSpeed(double s)
	{
		mSpeed = s;
	}

	double PointSegment::getSpeed() const
	{
		return mSpeed;
	}

	void PointSegment::setIsZMovement(bool isZ)
	{
		mIsZMovement = isZ;
	}

	bool PointSegment::isZMovement() const
	{
		return mIsZMovement;
	}

	void PointSegment::setIsMetric(bool isMetric)
	{
		mIsMetric = isMetric;
	}

	bool PointSegment::isMetric() const
	{
		return mIsMetric;
	}

	void PointSegment::setIsArc(bool isA)
	{
		mIsArc = isA;
	}

	bool PointSegment::isArc() const
	{
		return mIsArc;
	}

	void PointSegment::setIsRapidMovement(bool isF)
	{
		mIsRapidMovement = isF;
	}

	bool PointSegment::isRapidMovement() const
	{
		return mIsRapidMovement;
	}

	// Arc properties.

	void PointSegment::setArcCenter(const vec3& center)
	{
		mArcProperties.center = vec3(center);
		setIsArc(true);
	}

	vector<double> PointSegment::centerPoints() const
	{
		vector<double> points;
		points.push_back(mArcProperties.center.x);
		points.push_back(mArcProperties.center.y);
		points.push_back(mArcProperties.center.z);
		return points;
	}

	vec3 PointSegment::center() const
	{
		return mArcProperties.center;
	}

	void PointSegment::setIsClockwise(bool clockwise)
	{
		mArcProperties.isClockwise = clockwise;
	}

	bool PointSegment::isClockwise() const
	{
		return mArcProperties.isClockwise;
	}

	void PointSegment::setRadius(double rad)
	{
		mArcProperties.radius = rad;
	}

	double PointSegment::getRadius() const
	{
		return mArcProperties.radius;
	}

	void PointSegment::convertToMetric()
	{
		if (mIsMetric)
		{
			return;
		}

		mIsMetric = true;
		mPoint.x = mPoint.x * 25.4;
		mPoint.y = mPoint.y * 25.4;
		mPoint.z = mPoint.z * 25.4;

		if (mIsArc)
		{
			mArcProperties.center.x = mArcProperties.center.x * 25.4;
			mArcProperties.center.y = mArcProperties.center.y * 25.4;
			mArcProperties.center.z = mArcProperties.center.z * 25.4;
			mArcProperties.radius *= 25.4;
		}
	}

	bool PointSegment::isAbsolute() const
	{
		return mIsAbsolute;
	}

	void PointSegment::setIsAbsolute(bool isAbsolute)
	{
		mIsAbsolute = isAbsolute;
	}

	PointSegment::planes PointSegment::plane() const
	{
		return mPlane;
	}

	void PointSegment::setPlane(const planes &plane)
	{
		mPlane = plane;
	}

	double PointSegment::getSpindleSpeed() const
	{
		return mSpindleSpeed;
	}

	void PointSegment::setSpindleSpeed(double spindleSpeed)
	{
		mSpindleSpeed = spindleSpeed;
	}

	double PointSegment::getDwell() const
	{
		return mDwell;
	}

	void PointSegment::setDwell(double dwell)
	{
		mDwell = dwell;
	}

	GCodeCommand* PointSegment::getParent() const
	{
		return mParent;
	}

	void PointSegment::setParent(GCodeCommand* parent)
	{
		mParent = parent;
	}
}
