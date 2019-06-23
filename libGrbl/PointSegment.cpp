// This file is a part of "CoconutCNC" application.
// This file was originally ported from "PointSegment.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "PointSegment.h"

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
		mPoint(vec3(0.0)
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
	//    qDebug() << "PointSegment: Copy Constructor, isZMovement" << mIsZMovement;
		if (isArc())
		{
			//qDebug() << "PointSegment: isArc";
			setArcCenter(ps.center());
			setRadius(ps.getRadius());
			setIsClockwise(ps.isClockwise());
			mPlane = ps.plane();
		}
	}

	PointSegment::PointSegment(GCodeCommand* parent, const QVector3D &b, int num)
		: PointSegment(parent)

	{
	//    qDebug() << "PointSegment: QVector3D Constructor, " << b << num;
		mPoint = QSharedPointer<QVector3D>::create(b);
		mLineNumber = num;
	}

	PointSegment::PointSegment
	(
		GCodeCommand* parent,
		const QVector3D &point,
		int num,
		const QVector3D &center,
		double radius,
		bool clockwise
	) : PointSegment(parent, point, num)
	{
		mIsArc = true;
		mArcProperties.center = QVector3D(center);
		mArcProperties.radius = radius;
		mArcProperties.isClockwise = clockwise;
	}

	PointSegment::~PointSegment()
	{
	}

	QVector3D* PointSegment::getPointHandle()
	{
		return mPoint.data();
	}

	QVector<double> PointSegment::points() const
	{
		QVector<double> points;
		points.append(mPoint->x());
		points.append(mPoint->y());
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

	void PointSegment::setArcCenter(const QVector3D& center)
	{
		mArcProperties.center = QVector3D(center);
		setIsArc(true);
	}

	QVector<double> PointSegment::centerPoints() const
	{
		QVector<double> points;
		points.append(mArcProperties.center.x());
		points.append(mArcProperties.center.y());
		points.append(mArcProperties.center.z());
		return points;
	}

	QVector3D PointSegment::center() const
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
		mPoint->setX(mPoint->x() * 25.4);
		mPoint->setY(mPoint->y() * 25.4);
		mPoint->setZ(mPoint->z() * 25.4);

		if (mIsArc)
		{
			mArcProperties.center.setX(mArcProperties.center.x() * 25.4);
			mArcProperties.center.setY(mArcProperties.center.y() * 25.4);
			mArcProperties.center.setZ(mArcProperties.center.z() * 25.4);
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
