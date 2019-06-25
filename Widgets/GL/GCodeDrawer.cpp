// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GCodeDrawer.h"

namespace Coconut
{
	GCodeDrawer::GCodeDrawer(AppState* state)
		: GLWidget(state,"GCodeDrawer", true),
		  mIgnoreZ(false),
		  mColorNormal(vec4(0,0,1,1)),
		  mColorDrawn(vec4(0,0,0,0)),
		  mColorHighlight(vec4(1,0,0,1)),
		  mColorZMovement(vec4(1,1,0,1)),
		  mColorStart(vec4(0,1,0,1)),
		  mColorEnd(vec4(1,0,0,1)),
		  mColorArc(vec4(0,1,0,1)),
		  mColorRapid(vec4(1,0,0,1)),
		  mGeometryUpdated(false)
	{
		mPosition = vec3(0,0,0);
		mViewParser = nullptr;
	}

	void GCodeDrawer::Update()
	{
		mIndexes.clear();
		mGeometryUpdated = false;
	}

	void GCodeDrawer::Update(const vector<int>& indexes)
	{
		// Store segments to Update
		mIndexes.insert(mIndexes.end(),indexes.begin(),indexes.end());
	}

	bool GCodeDrawer::UpdateData()
	{
		if (mIndexes.empty())
		{
			return PrepareVectors();
		}
		else
		{
			return UpdateVectors();
		}
	}

	bool GCodeDrawer::PrepareVectors()
	{
		debug("GCodeDrawer: preparing vectors");

		if (mViewParser == nullptr)
		{
			debug("GCodeDrawer: Viewparser is null");
			return true;
		}

		vector<LineSegment*> list = mViewParser->getLineSegmentHandlesList();
		GLWidgetVertex vertex;

		debug("GCodeDrawer: lines count {}",  list.size());

		// Clear all vertex data
        ClearLineVertexBuffer();
        ClearPointVertexBuffer();
        ClearTriangleVertexBuffer();


		bool drawFirstPoint = true;
		for (int i = 0; i < list.size(); i++)
		{
			LineSegment* ls = list.at(i);
			if (isnan(ls->getEnd().z))
			{
				continue;
			}

			// Find first point of toolpath
			if (drawFirstPoint)
			{

				if (isnan(ls->getEnd().x) || isnan(ls->getEnd().y))
				{
					continue;
				}

				// Draw first toolpath point
				vertex.Color = mColorStart;
				vertex.Position = ls->getEnd() + mPosition;
				if (mIgnoreZ)
				{
					vertex.Position.z = (0);
				}
				AddPointVertex(vertex);

				drawFirstPoint = false;
				continue;
			}

			// Prepare vertices
			/*
			if (ls->isRapidMovement())
			{
				vertex.start = ls->getStart();
			}
			else
			{
				vertex.start = vec3(NAN, NAN, NAN);
			}
				y tho?
			*/



			ls->setVertexIndex(mLineVertexBuffer.size());

			// Set color
			vertex.Color = GetSegmentColorVector(ls);

			// Line start
			vertex.Position = ls->getStart() + mPosition;
			if (mIgnoreZ) vertex.Position.z = (0);
			AddLineVertex(vertex);

			// Line end
			vertex.Position = ls->getEnd() + mPosition;
			if (mIgnoreZ) vertex.Position.z = (0);
			AddLineVertex(vertex);

			// Draw last toolpath point
			if (i == list.size() - 1)
			{
				vertex.Color = mColorEnd;
				vertex.Position = ls->getEnd() + mPosition;

				if (mIgnoreZ)
				{
					vertex.Position.z = (0);
				}

				AddPointVertex(vertex);
			}
		}
		mGeometryUpdated = true;
		//mIndexes.clear();
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
			vertexIndex = ls->vertexIndex();
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



	vec4 GCodeDrawer::GetSegmentColorVector(LineSegment* segment)
	{
		return GetSegmentColor(segment);
	}

	vec4 GCodeDrawer::GetSegmentColor(LineSegment* segment)
	{

		if (segment->isZMovement())
		{
			return mColorZMovement;
		}
		else if (segment->isArc())
		{
			return mColorArc;
		}
		else if (segment->isRapidMovement())
		{
			return mColorRapid;
		}
		else if (segment->drawn())
		{
			return mColorDrawn;
		}
		else if (segment->isHightlight())
		{
			return mColorHighlight;
		}


		return mColorNormal;
	}

	vec3 GCodeDrawer::GetSizes() const
	{
		vec3 min = mViewParser->getMinimumExtremes();
		vec3 max = mViewParser->getMaximumExtremes();

		return vec3(max.x - min.x, max.y - min.y, max.z - min.z);
	}

	vec3 GCodeDrawer::GetMinimumExtremes() const
	{
		vec3 v = mViewParser->getMinimumExtremes();
		if (mIgnoreZ)
			v.z = (0);

		return v;
	}

	vec3 GCodeDrawer::GetMaximumExtremes() const
	{
		vec3 v = mViewParser->getMaximumExtremes();
		if (mIgnoreZ) v.z = (0);

		return v;
	}

	bool GCodeDrawer::Init()
	{
		mIgnoreZ = false;
		mColorNormal = vec4(0,0,1,1);
		mColorDrawn = vec4(0,0,0,1);
		mColorHighlight = vec4(1,0,0,1);
		mColorZMovement = vec4(1,1,0,1);
		mColorStart = vec4(0,1,0,1);
		mColorEnd = vec4(1,0,0,1);
		mGeometryUpdated = false;
		//mViewParser->clear();
		//mViewParser = GCodeViewParser(mAppState);
		Update();
        return true;
	}

	void GCodeDrawer::UpdateViewParser(GCodeParser* parser)
	{
		mViewParser->setLinesFromParser(parser,10,true);
		Update();
	}

	GCodeViewParser* GCodeDrawer::GetViewParserHandle()
	{
		return mViewParser;
	}

	bool GCodeDrawer::GeometryUpdated()
	{
		return mGeometryUpdated;
	}

	vec4 GCodeDrawer::ColorNormal() const
	{
		return mColorNormal;
	}

	void GCodeDrawer::SetColorNormal(const vec4 &colorNormal)
	{
		mColorNormal = colorNormal;
	}

	vec4 GCodeDrawer::ColorHighlight() const
	{
		return mColorHighlight;
	}

	void GCodeDrawer::SetColorHighlight(const vec4 &colorHighlight)
	{
		mColorHighlight = colorHighlight;
	}

	vec4 GCodeDrawer::ColorZMovement() const
	{
		return mColorZMovement;
	}

	void GCodeDrawer::SetColorZMovement(const vec4 &colorZMovement)
	{
		mColorZMovement = colorZMovement;
	}

	vec4 GCodeDrawer::ColorDrawn() const
	{
		return mColorDrawn;
	}

	void GCodeDrawer::SetColorDrawn(const vec4 &colorDrawn)
	{
		mColorDrawn = colorDrawn;
	}

	vec4 GCodeDrawer::ColorStart() const
	{
		return mColorStart;
	}

	void GCodeDrawer::SetColorStart(const vec4 &colorStart)
	{
		mColorStart = colorStart;
	}

	vec4 GCodeDrawer::ColorEnd() const
	{
		return mColorEnd;
	}

	void GCodeDrawer::SetColorEnd(const vec4 &colorEnd)
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

	vec3 GCodeDrawer::GetPosition() const
	{
		return mPosition;
	}

	void GCodeDrawer::SetPosition(vec3 position)
	{
		mPosition = position;
		Update();
	}

}
