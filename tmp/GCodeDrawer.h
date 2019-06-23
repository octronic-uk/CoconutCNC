// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include "GLWidget.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <LineSegment.h>
#include <GcodeViewParser.h>

using glm::vec3;
using glm::vec4;

namespace Coconut
{
	class GcodeDrawer : public GLWidGet
	{
	public:

		explicit GcodeDrawer();
		GcodeDrawer(const GcodeDrawer& other);

		void Update();
		void Update(const vector<int>& indexes);
		bool UpdateData() override;

		vec3 GetSizes() const;
		vec3 GetMinimumExtremes() const override;
		vec3 GetMaximumExtremes() const override;

		void Initialise();
		void UpdateViewParser(GcodeParser*);

		GcodeViewParser* GetViewParserHandle();

		bool Simplify() const;
		void SetSimplify(bool simplify);

		double SimplifyPrecision() const;
		void SetSimplifyPrecision(double simplifyPrecision);

		bool GeometryUpdated();

		vec4 ColorNormal() const;
		void SetColorNormal(const vec4 &colorNormal);

		vec4 ColorHighlight() const;
		void SetColorHighlight(const vec4 &colorHighlight);

		vec4 ColorZMovement() const;
		void SetColorZMovement(const vec4 &colorZMovement);

		vec4 ColorDrawn() const;
		void SetColorDrawn(const vec4 &colorDrawn);

		vec4 ColorStart() const;
		void SetColorStart(const vec4 &colorStart);

		vec4 ColorEnd() const;
		void SetColorEnd(const vec4 &colorEnd);

		bool GetIgnoreZ() const;
		void SetIgnoreZ(bool ignoreZ);

		GcodeParser GetParser() const;
		void SetParser(const GcodeParser& parser);

		vec3 GetPosition() const;
		void SetPosition(vec3 position);

	private:
		GcodeViewParser* mViewParser;
		bool mIgnoreZ;
		vec4 mColorNormal;
		vec4 mColorDrawn;
		vec4 mColorHighlight;
		vec4 mColorZMovement;
		vec4 mColorStart;
		vec4 mColorEnd;
		vec4 mColorArc;
		vec4 mColorRapid;
		vector<int> mIndexes;
		bool mGeometryUpdated;
		vec3 mPosition;

		bool prepareVectors();
		bool UpdateVectors();

		vec4 GetSegmentColorVector(LineSegment* segment);
		vec4 GetSegmentColor(LineSegment* segment);
	};

}
