// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "ToolDrawer.h"

#include <chrono>
#include "../../AppState.h"
#include "../../Model/Grbl/GrblMachineModel.h"
#include "../../Model/Settings/ToolSettings.h"
#include "../../Model/Settings/ToolHolderSettings.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

namespace Coconut
{

	ToolDrawer::ToolDrawer(AppState* state)
		: GLWidget(state,"ToolDrawer"),
		  mRotationAngle(0.0),
		  mToolColor(vec4(.9f, 0.54f, 0.f, 1.f)),
		  mToolHolderColor(vec4(.21f, .23f, .26f, 1.f)),
          mToolLength(0.f),
		  mSpindleRotating(false),
		  mSpindleSpeed(10.0f),
          mNeedsGeometryUpdate(true),
          mCurrentTime(0)

	{
		debug("ToolDrawer: Constructing");
	}

	ToolDrawer::~ToolDrawer()
	{
		debug("ToolDrawer: Destructing");
	}

	bool ToolDrawer::Init()
	{
        info("ToolDrawer: {}",__FUNCTION__);
        GLWidget::Init();
        ClearLineVertexBuffer();
        ClearTriangleVertexBuffer();

		GenerateToolGeometry();
		GenerateToolHolderGeometry();

		Rotate();

        SubmitTriangleVertexBuffer();
        return true;
    }

    void ToolDrawer::GenerateToolGeometry()
    {
        info("ToolDrawer: {}",__FUNCTION__);
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();
        int current_tool = grbl.GetToolNumber();
        info("ToolDrawer: Current tool {}",current_tool);
		if (current_tool > -1)
		{
			ToolSettings& tool = mAppState->GetSettingsModel().GetToolSettingsByToolNumber(current_tool);
            if (tool != ToolSettings::None)
            {
				vector<Cylinder>& tool_cylinders = tool.GetCylindersVector();
				info("ToolDrawer: Generating {} cylinders", tool_cylinders.size());
				vec3 origin = grbl.GetMachinePosition();
				for (const Cylinder& c : tool_cylinders)
				{
					AddTriangleVertices(GenerateCylinderGeometry(c,origin,mToolColor));
					origin.z += c.GetLength();
				}
                mToolLength = origin.z;
            }
            else
            {
                info("ToolDrawer: Tool number {} not found",current_tool);
            }
        }
    }

    void ToolDrawer::GenerateToolHolderGeometry()
    {
        info("ToolDrawer: {}",__FUNCTION__);
        GrblMachineModel& grbl = mAppState->GetGrblMachineModel();
        SettingsModel& settings = mAppState->GetSettingsModel();
        int current_tool = grbl.GetToolNumber();
        info("ToolDrawer: Current tool {}",current_tool);
		if (current_tool > -1)
		{
			ToolSettings& tool = settings.GetToolSettingsByToolNumber(current_tool);
            int tool_holder_id = tool.GetToolHolderID();
			ToolHolderSettings& tool_holder = settings.GetToolHolderSettingsByID(tool_holder_id);

            if (tool_holder != ToolHolderSettings::None)
            {
				vector<Cylinder>& tool_holder_cylinders = tool_holder.GetCylindersVector();
				info("ToolDrawer: Generating {} cylinders", tool_holder_cylinders.size());
				vec3 origin = grbl.GetMachinePosition();
                origin.z += mToolLength;

				for (const Cylinder& c : tool_holder_cylinders)
				{
					AddTriangleVertices(GenerateCylinderGeometry(c,origin,mToolHolderColor));
					origin.z += c.GetLength();
				}
            }
            else
            {
                info("ToolDrawer: Tool number {} not found",current_tool);
            }
        }
	}

	vector<GLWidgetVertex> ToolDrawer::GenerateCylinderGeometry
    (const Cylinder& cylinder, const vec3& origin, const vec4& color)
	{
        info("ToolDrawer: {}",__FUNCTION__);

        vector<GLWidgetVertex> vertices;

	   int slices = cylinder.GetFaces();
	   float upperRadius = cylinder.GetUpperDiameter()/2;
	   float lowerRadius = cylinder.GetLowerDiameter()/2;
	   float length = cylinder.GetLength();
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
	    mCurrentTime = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
        if(mNeedsGeometryUpdate)
        {
            ClearTriangleVertexBuffer();
            GenerateToolGeometry();
            GenerateToolHolderGeometry();
            SubmitTriangleVertexBuffer();
            mNeedsGeometryUpdate = false;
        }
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
        return mNeedsGeometryUpdate;
    }

    void ToolDrawer::SetNeedsGeometryUpdate(bool b)
    {
     mNeedsGeometryUpdate = b;
    }
}
