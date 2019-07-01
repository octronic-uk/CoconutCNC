// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include "GLWidget.h"

#include <glm/vec3.hpp>
#include "../../Model/Grbl/LineSegment.h"

using glm::vec3;

namespace Coconut
{
    class AppState;
	class GCodeDrawer : public GLWidget
	{
	public:

		GCodeDrawer(AppState* state);

        bool Init() override;
		void Update() override;

    	void ReadFromFileModel();
        void ClearState();

		bool Simplify() const;
		void SetSimplify(bool simplify);

		double SimplifyPrecision() const;
		void SetSimplifyPrecision(double simplifyPrecision);

		vec3 ColorNormal() const;
		void SetColorNormal(const vec3 &colorNormal);

		vec3 ColorHighlight() const;
		void SetColorHighlight(const vec3 &colorHighlight);

		vec3 ColorZMovement() const;
		void SetColorZMovement(const vec3 &colorZMovement);

		vec3 ColorDrawn() const;
		void SetColorDrawn(const vec3 &colorDrawn);

		vec3 ColorStart() const;
		void SetColorStart(const vec3 &colorStart);

		vec3 ColorEnd() const;
		void SetColorEnd(const vec3 &colorEnd);

		bool GetIgnoreZ() const;
		void SetIgnoreZ(bool ignoreZ);

    protected:
		bool PrepareVectors();
		bool UpdateVectors();
		vec3 GetSegmentColorVector(const LineSegment& segment);
		vec3 GetSegmentColor(const LineSegment& segment);

	private:
		bool mIgnoreZ;
		vec3 mColorNormal;
		vec3 mColorDrawn;
		vec3 mColorHighlight;
		vec3 mColorZMovement;
		vec3 mColorStart;
		vec3 mColorEnd;
		vec3 mColorArc;
		vec3 mColorRapid;
		vector<int> mIndexes;
	};
}
