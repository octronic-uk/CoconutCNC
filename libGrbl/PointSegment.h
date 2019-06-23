// This file is a part of "CoconutCNC" application.
// This file was originally ported from "PointSegment.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "ArcProperties.h"

using std::vector;

namespace Coconut
{
	class GCodeCommand;

	class PointSegment
	{
	public:
		enum planes
		{
			XY,
			ZX,
			YZ
		};

		PointSegment(GCodeCommand* parent);
		PointSegment(const PointSegment &ps);
		PointSegment(GCodeCommand* parent, const vec3 &b, int num);
		PointSegment(GCodeCommand* parent, const vec3 &getPointHandle,
			int num, const vec3 &center, double radius, bool clockwise);
		~PointSegment();

		vec3* getPointHandle();

		vector<double> points() const;

		void setToolHead(int head);
		int getToolhead() const;

		void setLineNumber(int num);
		int getLineNumber() const;

		void setSpeed(double s);
		double getSpeed() const;

		void setIsZMovement(bool isZ);
		bool isZMovement() const;

		void setIsMetric(bool mIsMetric);
		bool isMetric() const;

		void setIsArc(bool isA);
		bool isArc() const;

		void setIsRapidMovement(bool isF);
		bool isRapidMovement() const;

		void setArcCenter(const vec3 &center);

		vector<double> centerPoints() const;
		vec3 center() const;

		void setIsClockwise(bool clockwise);
		bool isClockwise() const;

		void setRadius(double rad);
		double getRadius() const;

		void convertToMetric();

		bool isAbsolute() const;
		void setIsAbsolute(bool isAbsolute);

		planes plane() const;
		void setPlane(const planes &plane);

		double getSpindleSpeed() const;
		void setSpindleSpeed(double spindleSpeed);

		double getDwell() const;
		void setDwell(double dwell);

		GCodeCommand* getParent() const;
		void setParent(GCodeCommand* parent);

	private:
		GCodeCommand* mParent;
		ArcProperties mArcProperties;
		int mToolhead;
		double mSpeed;
		double mSpindleSpeed;
		double mDwell;
		vec3 mPoint;
		bool mIsMetric;
		bool mIsZMovement;
		bool mIsArc;
		bool mIsRapidMovement;
		bool mIsAbsolute;
		int mLineNumber;
		planes mPlane;
	};

}
