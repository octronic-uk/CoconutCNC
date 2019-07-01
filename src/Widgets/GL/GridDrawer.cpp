/*
 * Grid.cpp
 *
 * Created: 07 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "GridDrawer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../Common/Logger.h"

namespace Coconut
{

    GridDrawer::GridDrawer
    (
		AppState* p, AxisPair xp, vec3 position,
		float majorSpacing, float minorSpacing, float size,
		vec3 majorColour, vec3 minorColour
    )
        : GLWidget(p, "Grid"), mAxisPair(xp), mTranslation(position), mSize(size),
          mMajorSpacing(majorSpacing), mMinorSpacing(minorSpacing),
          mMajorColour(majorColour), mMinorColour(minorColour)
    {
        debug("Grid: Constructor");
    }

    GridDrawer::~GridDrawer()
    {
        debug("Grid: Destructor");
    }

    bool GridDrawer::Init()
    {
        debug("Grid: {}",__FUNCTION__);
        if (!GLWidget::Init()) return false;
        RecalculateGridLines();
        return true;
    }

    void GridDrawer::Update()
    {

    }

    void
    GridDrawer::InitMajorGridData
    ()
    {
        debug("Grid: {}",__FUNCTION__);

        // Major Grid
        for (float step = 0; step <= mSize; step += mMajorSpacing)
        {
            GLWidgetVertex lineStart, lineEnd;

            lineStart.Color = mMajorColour;
            lineEnd.Color = mMajorColour;

            switch (mAxisPair)
            {
                case XZ:
                    // X
                    lineStart.Position = vec3(step, 0.0f, 0);
                    lineEnd.Position   = vec3(step, 0.0f, mSize);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    // Z
                    lineStart.Position = vec3(0, 0.0f, step);
                    lineEnd.Position   = vec3(mSize, 0.0f, step);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    break;
                case YZ:
                    // Y
                    lineStart.Position = vec3(0.0f, step, 0);
                    lineEnd.Position   = vec3(0.0f, step, mSize);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    // Z
                    lineStart.Position = vec3(0.0f, 0.0f, step);
                    lineEnd.Position   = vec3(0.0f, mSize, step);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    break;
                case XY:
                    // X
                    lineStart.Position = vec3(step, 0.0f, 0);
                    lineEnd.Position   = vec3(step, mSize, 0.0f);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    // Y
                    lineStart.Position = vec3(0, step, 0.0f);
                    lineEnd.Position   = vec3(mSize, step, 0.0f);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);

                    break;
            }
        }
    }

    void
    GridDrawer::InitMinorGridData
    ()
    {
        debug("Grid: {}",__FUNCTION__);

        // Major Grid
        for (float step = 0; step <= mSize; step += mMinorSpacing)
        {
            if (fmod(step,mMajorSpacing) == 0.0f) continue;

            GLWidgetVertex lineStart, lineEnd;
            lineStart.Color = mMinorColour;
            lineEnd.Color   = mMinorColour;

            switch (mAxisPair)
            {
            	case XZ:
                    // X
                    lineStart.Position = vec3(step, 0.0f, 0);
                    lineEnd.Position   = vec3(step, 0.0f, mSize);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    // Z
                    lineStart.Position = vec3(0, 0.0f, step);
                    lineEnd.Position   = vec3(mSize, 0.0f, step);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    break;
                case YZ:
                    // Y
                    lineStart.Position = vec3(0.0f, step, 0);
                    lineEnd.Position   = vec3(0.0f, step, mSize);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    // Z
                    lineStart.Position = vec3(0.0f, 0.0f, step);
                    lineEnd.Position   = vec3(0.0f, mSize, step);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    break;
                case XY:
                    // X
                    lineStart.Position = vec3(step, 0.0f, 0);
                    lineEnd.Position   = vec3(step, mSize, 0.0f);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    // Y
                    lineStart.Position = vec3(0, step, 0.0f);
                    lineEnd.Position   = vec3(mSize, step, 0.0f);
                    AddLineVertex(lineStart);
                    AddLineVertex(lineEnd);
                    break;
            }
        }
    }

    GridDrawer::AxisPair GridDrawer::GetAxisPair() const
    {
        return mAxisPair;
    }

    vec3 GridDrawer::GetMinorColour() const
    {
        return mMinorColour;
    }

    void GridDrawer::SetMinorColour(vec3 minorColour)
    {
        mMinorColour = minorColour;
    }

    void GridDrawer::SetAxisPair(GridDrawer::AxisPair axisPair)
    {
        mAxisPair = axisPair;
    }

    vec3 GridDrawer::GetMajorColour() const
    {
        return mMajorColour;
    }

    void GridDrawer::SetMajorColour(vec3 majorColour)
    {
        mMajorColour = majorColour;
    }

    glm::vec3 GridDrawer::GetTranslation() const
    {
        return mTranslation;
    }

    void GridDrawer::SetTranslation(vec3 translation)
    {
        mTranslation = translation;
        mModelMatrix = glm::translate(mat4(1.0f),mTranslation);
    }

    float
    GridDrawer::GetMajorSpacing
    ()
    {
        return mMajorSpacing;
    }

    void
    GridDrawer::SetMajorSpacing
    (float ms)
    {
        mMajorSpacing = ms < 1.0f ? 1.0f : ms;
    }

    float
    GridDrawer::GetMinorSpacing
    ()
    {
        return mMinorSpacing;
    }

    void
    GridDrawer::SetMinorSpacing
    (float ms)
    {
        mMinorSpacing = ms < 0.1f ? 0.1f : ms;
    }

    float GridDrawer::GetSize()
    {
        return mSize;
    }

    void GridDrawer::SetSize(float sz)
    {
        mSize = sz < 1.0f ? 1.0f : sz;
    }

    void GridDrawer::RecalculateGridLines()
    {
        debug("Grid: {}",__FUNCTION__);
        mLineVertexBuffer.clear();
        ClearLineVertexBuffer();
        InitMajorGridData();
        InitMinorGridData();
        SubmitLineVertexBuffer();
    }
}
