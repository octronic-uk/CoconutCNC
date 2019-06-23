// This file is a part of "CoconutCNC" application.
// This file was originally ported from "LineSegment.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "PointSegment.h"

using glm::vec3;
using std::vector;

namespace Coconut
{
	class LineSegment
	{
	public:
		LineSegment();
		LineSegment(const vec3 &a, const vec3 &b, int num);
		LineSegment(const LineSegment &initial);
		~LineSegment();

		int getLineNumber() const;
		vector<vec3> getPointArray() const;
		vector<double> getPoints() const;

		vec3 getStart() const;
		void setStart(const vec3 &vector);

		vec3 getEnd() const;
		void setEnd(const vec3 &vector);

		void setToolHead(int head);
		int getToolhead() const;

		void setSpeed(double s);
		double getSpeed() const;

		void setIsZMovement(bool isZ);
		bool isZMovement() const;

		void setIsArc(bool isA);
		bool isArc() const;

		void setIsRapidMovement(bool isF);
		bool isRapidMovement() const;

		bool contains(const vec3 &point) const;

		void setDrawn(bool drawn);
		bool drawn() const;

		void setIsMetric(bool isMetric);
		bool isMetric() const;

		void setIsAbsolute(bool isAbsolute);
		bool isAbsolute() const;

		void setIsHightlight(bool isHightlight);
		bool isHightlight() const;

		void setVertexIndex(int vertexIndex);
		int vertexIndex() const;

		void setSpindleSpeed(double spindleSpeed);
		double getSpindleSpeed() const;

		void setDwell(double dwell);
		double getDwell() const;

		void setIsClockwise(bool isClockwise);
		bool isClockwise() const;

		void setPlane(const PointSegment::planes &plane);
		PointSegment::planes plane() const;

	private:
		int mToolhead;
		double mSpeed;
		double mSpindleSpeed;
		double mDwell;
		vec3 mFirst;
		vec3 mSecond;

		// Line properties
		bool mIsZMovement;
		bool mIsArc;
		bool mIsClockwise;
		bool mIsRapidMovement;
		int mLineNumber;
		bool mDrawn;
		bool mIsMetric;
		bool mIsAbsolute;
		bool mIsHightlight;
		int mVertexIndex;

		PointSegment::planes mPlane;
	};
}
