// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "ToolDrawer.h"

#include "../../Model/Settings/ToolSettings.h"
#include "../../Model/Settings/ToolHolderSettings.h"

namespace Coconut
{

	ToolDrawer::ToolDrawer(AppState* state)
		: GLWidget(state,"ToolDrawer"),
		  mToolPosition(vec3(0, 0, 0)),
		  mRotationAngle(0.0),
		  mToolColor(vec4(0.5)),
		  mToolHolderColor(vec4(0.5)),
		  mTool(nullptr),
		  mSpindleRotating(false),
		  mSpindleSpeed(10.0f),
          mNeedsGeometryUpdate(true)

	{
		debug("ToolDrawer: Constructing");
	}

	ToolDrawer::~ToolDrawer()
	{
		debug("ToolDrawer: Destructing");
	}

	bool ToolDrawer::Init()
	{
        ClearLineVertexBuffer();
        ClearTriangleVertexBuffer();

		if (!mTool)	return true;

		GenerateToolGeometry();
		GenerateToolHolderGeometry();

		Rotate();

        return true;
    }

    void ToolDrawer::GenerateToolGeometry()
    {

    }

    void ToolDrawer::GenerateToolHolderGeometry()
    {

	}

	vector<GLWidgetVertex> ToolDrawer::GenerateCylinderGeometry
    (Cylinder* cylinder, const vec3& origin, const vec4& color)
	{
        debug("ToolDrawer: {}",__FUNCTION__);

        vector<GLWidgetVertex> vertices;
	    if (!cylinder)
	    {
			debug("ToolDrawer: No Cylinder passed");
		    return vertices;
	    }

	   int slices = cylinder->GetFaces();
	   float upperRadius = cylinder->GetUpperDiameter()/2;
	   float lowerRadius = cylinder->GetLowerDiameter()/2;
	   float length = cylinder->GetLength();
	   float twoPi = 2 * static_cast<float>(M_PI);

	   for(int i=0; i<slices; i++)
	   {
			debug("ToolDrawer: Making a slice {}", i);

			float theta = ((twoPi / slices) * i ) + mRotationAngle;
			float nextTheta = ((twoPi / slices) * (i+1)) + mRotationAngle;

			debug("ToolDrawer: theta {}",theta);
			debug("ToolDrawer: nextTheta {}",nextTheta);

            float cos_theta = cos(theta);
            float sin_theta = sin(theta);

            float cos_nextTheta = cos(nextTheta);
            float sin_nextTheta = sin(nextTheta);

			/* vertex at top middle */
			vec3 topMiddlePos(origin.x, origin.y, origin.z + length);
			GLWidgetVertex topMiddle;
			topMiddle.Color = color;
			topMiddle.Position = topMiddlePos;
			debug("ToolDrawer: topMiddle {} {} {}", topMiddlePos.x, topMiddlePos.y, topMiddlePos.z);

			/* vertices at top edges of circle*/
			vec3 topEdge1Pos(origin.x + upperRadius*cos_theta,
	   			origin.y + upperRadius*sin_theta, origin.z + length);

			GLWidgetVertex topEdge1;
			topEdge1.Color = color;
			topEdge1.Position = topEdge1Pos;
			debug("ToolDrawer: topEdge1 {} {} {}", topEdge1Pos.x,topEdge1Pos.y,topEdge1Pos.z);

			vec3 topEdge2Pos(origin.x + upperRadius*cos_nextTheta,
				origin.y + upperRadius*sin_nextTheta, origin.z + length);

			GLWidgetVertex topEdge2;
			topEdge2.Color = color;
			topEdge2.Position = topEdge2Pos;
			debug("ToolDrawer: topEdge2 {} {} {}", topEdge2Pos.x,topEdge2Pos.y,topEdge2Pos.z);

			/* vertex at bottom middle */
			vec3 bottomMiddlePos( origin.x, origin.y, origin.z);
			GLWidgetVertex bottomMiddle;
			bottomMiddle.Color = color;
			bottomMiddle.Position = bottomMiddlePos;
			debug("ToolDrawer: bottomMiddle {} {} {}", bottomMiddlePos.x,bottomMiddlePos.y,bottomMiddlePos.z);

			/* vertices at bottom edges of circle*/
			vec3 bottomEdge1Pos(origin.x + lowerRadius*cos_theta,
				origin.y + lowerRadius*sin_theta, origin.z);

			GLWidgetVertex bottomEdge1;
			bottomEdge1.Color = color;
			bottomEdge1.Position = bottomEdge1Pos;
			debug("ToolDrawer: bottomEdge1 {} {} {}", bottomEdge1Pos.x,bottomEdge1Pos.y,bottomEdge1Pos.z);

			vec3 bottomEdge2Pos(origin.x + lowerRadius*cos_nextTheta,
				origin.y + lowerRadius*sin(nextTheta), origin.z);

			GLWidgetVertex bottomEdge2;
			bottomEdge2.Color = color;
			bottomEdge2.Position = bottomEdge2Pos;
			debug("ToolDrawer: bottomEdge2 {} {} {}", bottomEdge2Pos.x,bottomEdge2Pos.y,bottomEdge2Pos.z);

			// Top
			vertices.push_back(topMiddle);
			vertices.push_back(topEdge1);
			vertices.push_back(topEdge2);

			// Side1
			vertices.push_back(topEdge2);
			vertices.push_back(topEdge1);
			vertices.push_back(bottomEdge1);

			// Side2
			vertices.push_back(bottomEdge1);
			vertices.push_back(bottomEdge2);
			vertices.push_back(topEdge2);

			// Bottom
			vertices.push_back(bottomMiddle);
			vertices.push_back(bottomEdge1);
			vertices.push_back(bottomEdge2);
		}
		return vertices;
	}

	void ToolDrawer::Update()
	{
		Rotate();
		return;
	}

	void ToolDrawer::SetSpindleRotating(bool rotating)
	{
	   mSpindleRotating = rotating;
	}

	void ToolDrawer::SetSpindleSpeed(float speed)
	{
	   mSpindleSpeed = speed;
	}

	ToolSettings* ToolDrawer::GetTool() const
	{
		return mTool;
	}

	void ToolDrawer::SetTool(ToolSettings* toolHandle)
	{
		mTool = toolHandle;
	}

	vec4 ToolDrawer::ToolColor() const
	{
		return mToolColor;
	}

	void ToolDrawer::SetToolColor(const vec4& color)
	{
		mToolColor = color;
	}

    vec4 ToolDrawer::ToolHolderColor() const
	{
		return mToolHolderColor;
	}

	void ToolDrawer::SetToolHolderColor(const vec4& color)
	{
		mToolHolderColor = color;
	}

    vec3 ToolDrawer::ToolPosition() const
	{
		return mToolPosition;
	}

	void ToolDrawer::SetToolPosition(const vec3 &toolPosition)
	{
		if (mToolPosition != toolPosition)
		{
			mToolPosition = toolPosition;
			Update();
		}
	}

	void ToolDrawer::Rotate()
	{
		mRotationAngle = NormalizeAngle(mRotationAngle + mSpindleSpeed);
	}

	float ToolDrawer::NormalizeAngle(float angle)
	{
		while (angle < 0)   angle += 360;
		while (angle > 360) angle -= 360;
		return angle;
	}

	bool ToolDrawer::NeedsUpdateGeometry() const
	{
		return mNeedsGeometryUpdate || mSpindleRotating;
	}
}
