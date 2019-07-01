// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GCodeDrawer.h"
#include "../../AppState.h"
#include "../../Model/Grbl/GCodeFileModel.h"
#include "../../Model/Grbl/GCodeViewParser.h"

namespace Coconut
{
	GCodeDrawer::GCodeDrawer(AppState* state)
		: GLWidget(state,"GCodeDrawer", true),
		  mIgnoreZ(false),
		  mColorNormal(vec3(0,0,1)),
		  mColorDrawn(vec3(0,0,0)),
		  mColorHighlight(vec3(1,0,0)),
		  mColorZMovement(vec3(1,1,0)),
		  mColorStart(vec3(0,1,0)),
		  mColorEnd(vec3(1,0,0)),
		  mColorArc(vec3(0,1,0)),
		  mColorRapid(vec3(1,0,0))
	{
	}

	bool GCodeDrawer::Init()
	{
        GLWidget::Init();
        return true;
	}

    void GCodeDrawer::ReadFromFileModel()
    {
		mIgnoreZ = false;
		mColorNormal = vec3(0,0,1);
		mColorDrawn = vec3(0,0,0);
		mColorHighlight = vec3(1,0,0);
		mColorZMovement = vec3(1,1,0);
		mColorStart = vec3(0,1,0);
		mColorEnd = vec3(1,0,0);
        PrepareVectors();
    }

    void GCodeDrawer::ClearState()
    {
    	ClearLineVertexBuffer();
        ClearPointVertexBuffer();
        ClearTriangleVertexBuffer();
    }

    void GCodeDrawer::Update()
    {

    }

	bool GCodeDrawer::PrepareVectors()
	{
        vec3 position(0.0f);
        GCodeViewParser& mViewParser = mAppState->GetGCodeFileModel().GetGCodeViewParser();
		debug("GCodeDrawer: preparing vectors");

		vector<LineSegment> list = mViewParser.GetLineSegmentHandlesList();
		GLWidgetVertex vertex;

		debug("GCodeDrawer: lines count {}",  list.size());

		// Clear all vertex data
        ClearLineVertexBuffer();
        ClearPointVertexBuffer();
        ClearTriangleVertexBuffer();

		bool drawFirstPoint = true;
		for (int i = 0; i < list.size(); i++)
		{
			LineSegment ls = list.at(i);
			if (isnan(ls.getEnd().z))
			{
				continue;
			}

			// Find first point of toolpath
			if (drawFirstPoint)
			{
				if (isnan(ls.getEnd().x) || isnan(ls.getEnd().y))
				{
					continue;
				}

				// Draw first toolpath point
				vertex.Color = mColorStart;
				vertex.Position = ls.getEnd() + position;
				if (mIgnoreZ)
				{
					vertex.Position.z = (0);
				}
				AddPointVertex(vertex);

				drawFirstPoint = false;
				continue;
			}

			ls.setVertexIndex(mLineVertexBuffer.size());

			// Set color
			vertex.Color = GetSegmentColorVector(ls);

			// Line start
			vertex.Position = ls.getStart() + position;
			if (mIgnoreZ) vertex.Position.z = (0);
			AddLineVertex(vertex);

			// Line end
			vertex.Position = ls.getEnd() + position;
			if (mIgnoreZ) vertex.Position.z = (0);
			AddLineVertex(vertex);

			// Draw last toolpath point
			if (i == list.size() - 1)
			{
				vertex.Color = mColorEnd;
				vertex.Position = ls.getEnd() + position;

				if (mIgnoreZ)
				{
					vertex.Position.z = (0);
				}

				AddPointVertex(vertex);
			}
		}

        SubmitPointVertexBuffer();
        SubmitLineVertexBuffer();
		return true;
	}

	bool GCodeDrawer::UpdateVectors()
	{
        /*
		if (mViewParser == nullptr)
		{
			return true;
		}

		// Update vertices
		vector<LineSegment*> list = mViewParser->getLineSegmentHandlesList();

		// Map buffer
		GLWidgetVertex *data = static_cast<GLWidgetVertexData*>(mVBO.map(QOpenGLBuffer::WriteOnly));

		// Update vertices for each line segment
		int vertexIndex;
		foreach (int i, mIndexes)
		{
			LineSegment *ls = list.at(i);

			// Update vertex pair
			if (i < 0 || i > list.count() - 1)
				continue;
			vertexIndex = ls.vertexIndex();
			if (vertexIndex >= 0)
			{
				// Update vertex array
				if (data)
				{
					data[vertexIndex].Color = getSegmentColorVector(ls);
					data[vertexIndex + 1].Color = data[vertexIndex].Color;
				}
				else
				{
					mLines[vertexIndex].Color = getSegmentColorVector(ls);
					mLines[vertexIndex + 1].Color = mLines.at(vertexIndex).Color;
				}
			}
		}

		//mIndexes.clear();
		if (data) mVBO.unmap();

		return !data;
        */
        return false;
	}



	vec3 GCodeDrawer::GetSegmentColorVector(const LineSegment& segment)
	{
		return GetSegmentColor(segment);
	}

	vec3 GCodeDrawer::GetSegmentColor(const LineSegment& segment)
	{

		if (segment.isZMovement())
		{
			return mColorZMovement;
		}
		else if (segment.isArc())
		{
			return mColorArc;
		}
		else if (segment.isRapidMovement())
		{
			return mColorRapid;
		}
		else if (segment.drawn())
		{
			return mColorDrawn;
		}
		else if (segment.isHightlight())
		{
			return mColorHighlight;
		}


		return mColorNormal;
	}

	vec3 GCodeDrawer::ColorNormal() const
	{
		return mColorNormal;
	}

	void GCodeDrawer::SetColorNormal(const vec3 &colorNormal)
	{
		mColorNormal = colorNormal;
	}

	vec3 GCodeDrawer::ColorHighlight() const
	{
		return mColorHighlight;
	}

	void GCodeDrawer::SetColorHighlight(const vec3 &colorHighlight)
	{
		mColorHighlight = colorHighlight;
	}

	vec3 GCodeDrawer::ColorZMovement() const
	{
		return mColorZMovement;
	}

	void GCodeDrawer::SetColorZMovement(const vec3 &colorZMovement)
	{
		mColorZMovement = colorZMovement;
	}

	vec3 GCodeDrawer::ColorDrawn() const
	{
		return mColorDrawn;
	}

	void GCodeDrawer::SetColorDrawn(const vec3 &colorDrawn)
	{
		mColorDrawn = colorDrawn;
	}

	vec3 GCodeDrawer::ColorStart() const
	{
		return mColorStart;
	}

	void GCodeDrawer::SetColorStart(const vec3 &colorStart)
	{
		mColorStart = colorStart;
	}

	vec3 GCodeDrawer::ColorEnd() const
	{
		return mColorEnd;
	}

	void GCodeDrawer::SetColorEnd(const vec3 &colorEnd)
	{
		mColorEnd = colorEnd;
	}

	bool GCodeDrawer::GetIgnoreZ() const
	{
		return mIgnoreZ;
	}

	void GCodeDrawer::SetIgnoreZ(bool ignoreZ)
	{
		mIgnoreZ = ignoreZ;
	}
}
