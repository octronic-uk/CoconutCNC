// This file Is a part of "CoconutCNC" application.
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

		PointSegment(const GCodeCommand& parent);
		PointSegment(const PointSegment& ps);
		PointSegment(const GCodeCommand& parent, const vec3 &b, int num);
		PointSegment(const GCodeCommand& parent, const vec3 &GetPointHandle, int num, const vec3 &center, float radius, bool clockwise);
		~PointSegment();

        PointSegment& operator=(const PointSegment&);

		vec3& GetPoint();

		vector<float> Points() const;

		void SetToolHead(int head);
		int GetToolhead() const;

		void SetLineNumber(int num);
		int GetLineNumber() const;

		void SetSpeed(float s);
		float GetSpeed() const;

		void SetIsZMovement(bool isZ);
		bool IsZMovement() const;

		void SetIsMetric(bool mIsMetric);
		bool IsMetric() const;

		void SetIsArc(bool isA);
		bool IsArc() const;

		void SetIsRapidMovement(bool isF);
		bool IsRapidMovement() const;

		void SetArcCenter(const vec3 &center);

		vector<float> CenterPoints() const;
		vec3& Center();

		void SetIsClockwise(bool clockwise);
		bool IsClockwise() const;

		void SetRadius(float rad);
		float GetRadius() const;

		void ConvertToMetric();

		bool IsAbsolute() const;
		void SetIsAbsolute(bool IsAbsolute);

		planes Plane() const;
		void SetPlane(const planes &plane);

		float GetSpindleSpeed() const;
		void SetSpindleSpeed(float spindleSpeed);

		float GetDwell() const;
		void SetDwell(float dwell);

		const GCodeCommand& GetParent() const;

	private:
		const GCodeCommand& mParent;
		ArcProperties mArcProperties;
		int mToolhead;
		float mSpeed;
		float mSpindleSpeed;
		float mDwell;
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
