// This file is a part of "CoconutCNC" application.
// This file was originally ported from "PointSegment.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "PointSegment.h"
#include "../../Common/Logger.h"

namespace Coconut
{
	PointSegment::PointSegment(const GCodeCommand& parent) :
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
          mParent(ps.mParent),
          mArcProperties(ps.mArcProperties),
		  mToolhead(ps.mToolhead),
		  mSpeed(ps.mSpeed),
          mSpindleSpeed(ps.mSpindleSpeed),
          mDwell(ps.mDwell),
		  mPoint(ps.mPoint),
		  mIsMetric(ps.mIsMetric),
		  mIsZMovement(ps.mIsZMovement),
		  mIsArc(ps.mIsArc),
		  mIsRapidMovement(ps.mIsRapidMovement),
		  mIsAbsolute(ps.mIsAbsolute),
		  mLineNumber(ps.mLineNumber),
          mPlane(ps.mPlane)
	{
	    debug("PointSegment: Copy Constructor, isZMovement {}" ,mIsZMovement);

		if (IsArc())
		{
			//qDebug() << "PointSegment: isArc";
			SetArcCenter(ps.mArcProperties.center);
			SetRadius(ps.mArcProperties.radius);
			SetIsClockwise(ps.IsClockwise());
			mPlane = ps.Plane();
		}
	}



	PointSegment::PointSegment(const GCodeCommand& parent, const vec3 &b, int num)
		: PointSegment(parent)

	{
	    debug("PointSegment: vec3 Constructor, {} {} {} / {}" , b.x, b.y, b.z,  num);
		mPoint = vec3(b);
		mLineNumber = num;
	}

	PointSegment::PointSegment
	(
		const GCodeCommand& parent,
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


	PointSegment& PointSegment::operator=(const PointSegment& ps)
	{
		//mParent = ps.mParent;
        mArcProperties = ps.mArcProperties;
		mToolhead = ps.mToolhead;
		mSpeed = ps.mSpeed;
        mSpindleSpeed = ps.mSpindleSpeed,
        mDwell = ps.mDwell;
		mPoint = ps.mPoint;
		mIsMetric = ps.mIsMetric;
		mIsZMovement = ps.mIsZMovement;
		mIsArc = ps.mIsArc;
		mIsRapidMovement = ps.mIsRapidMovement;
		mIsAbsolute = ps.mIsAbsolute;
		mLineNumber = ps.mLineNumber;
        mPlane = ps.mPlane;
        return *this;
    }

	PointSegment::~PointSegment()
	{
	}

	vec3& PointSegment::GetPoint()
	{
		return mPoint;
	}

	vector<double> PointSegment::Points() const
	{
		vector<double> points;
		points.push_back(mPoint.x);
		points.push_back(mPoint.y);
		return points;
	}

	void PointSegment::SetToolHead(int head)
	{
		mToolhead = head;
	}

	int PointSegment::GetToolhead() const
	{
		return mToolhead;
	}

	void PointSegment::SetLineNumber(int num)
	{
		mLineNumber = num;
	}

	int PointSegment::GetLineNumber() const
	{
		return mLineNumber;
	}

	void PointSegment::SetSpeed(double s)
	{
		mSpeed = s;
	}

	double PointSegment::GetSpeed() const
	{
		return mSpeed;
	}

	void PointSegment::SetIsZMovement(bool isZ)
	{
		mIsZMovement = isZ;
	}

	bool PointSegment::IsZMovement() const
	{
		return mIsZMovement;
	}

	void PointSegment::SetIsMetric(bool isMetric)
	{
		mIsMetric = isMetric;
	}

	bool PointSegment::IsMetric() const
	{
		return mIsMetric;
	}

	void PointSegment::SetIsArc(bool isA)
	{
		mIsArc = isA;
	}

	bool PointSegment::IsArc() const
	{
		return mIsArc;
	}

	void PointSegment::SetIsRapidMovement(bool isF)
	{
		mIsRapidMovement = isF;
	}

	bool PointSegment::IsRapidMovement() const
	{
		return mIsRapidMovement;
	}

	// Arc properties.

	void PointSegment::SetArcCenter(const vec3& center)
	{
		mArcProperties.center = vec3(center);
		SetIsArc(true);
	}

	vector<double> PointSegment::CenterPoints() const
	{
		vector<double> points;
		points.push_back(mArcProperties.center.x);
		points.push_back(mArcProperties.center.y);
		points.push_back(mArcProperties.center.z);
		return points;
	}

	vec3& PointSegment::Center()
	{
		return mArcProperties.center;
	}

	void PointSegment::SetIsClockwise(bool clockwise)
	{
		mArcProperties.isClockwise = clockwise;
	}

	bool PointSegment::IsClockwise() const
	{
		return mArcProperties.isClockwise;
	}

	void PointSegment::SetRadius(double rad)
	{
		mArcProperties.radius = rad;
	}

	double PointSegment::GetRadius() const
	{
		return mArcProperties.radius;
	}

	void PointSegment::ConvertToMetric()
	{
		if (mIsMetric)
		{
			return;
		}

		mIsMetric = true;
		mPoint.x = mPoint.x * 25.4f;
		mPoint.y = mPoint.y * 25.4f;
		mPoint.z = mPoint.z * 25.4f;

		if (mIsArc)
		{
			mArcProperties.center.x = mArcProperties.center.x * 25.4f;
			mArcProperties.center.y = mArcProperties.center.y * 25.4f;
			mArcProperties.center.z = mArcProperties.center.z * 25.4f;
			mArcProperties.radius *= 25.4f;
		}
	}

	bool PointSegment::IsAbsolute() const
	{
		return mIsAbsolute;
	}

	void PointSegment::SetIsAbsolute(bool isAbsolute)
	{
		mIsAbsolute = isAbsolute;
	}

	PointSegment::planes PointSegment::Plane() const
	{
		return mPlane;
	}

	void PointSegment::SetPlane(const planes &plane)
	{
		mPlane = plane;
	}

	double PointSegment::GetSpindleSpeed() const
	{
		return mSpindleSpeed;
	}

	void PointSegment::SetSpindleSpeed(double spindleSpeed)
	{
		mSpindleSpeed = spindleSpeed;
	}

	double PointSegment::GetDwell() const
	{
		return mDwell;
	}

	void PointSegment::SetDwell(double dwell)
	{
		mDwell = dwell;
	}

	const GCodeCommand& PointSegment::GetParent() const
	{
		return mParent;
	}
}
