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
        float halfSize = mSize/2.0f;

        // Major Grid
        for (float axis = mMajorSpacing; axis <= halfSize; axis += mMajorSpacing)
        {
            GLWidgetVertex posStart, posEnd, negStart, negEnd;
            posStart.Color = mMajorColour;
            posEnd.Color = mMajorColour;
            negStart.Color = mMajorColour;
            negEnd.Color = mMajorColour;

            switch (mAxisPair)
            {
                case XZ:
                    // X
                    posStart.Position = vec3( axis, 0.0f,-halfSize);
                    posEnd.Position   = vec3( axis, 0.0f, halfSize);
                    negStart.Position = vec3(-axis, 0.0f,-halfSize);
                    negEnd.Position   = vec3(-axis, 0.0f, halfSize);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    // Z
                    posStart.Position = vec3(-halfSize, 0.0f, axis);
                    posEnd.Position   = vec3( halfSize, 0.0f, axis);
                    negStart.Position = vec3(-halfSize, 0.0f,-axis);
                    negEnd.Position   = vec3( halfSize, 0.0f,-axis);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    break;
                case YZ:
                    // Y
                    posStart.Position = vec3(0.0f, axis,-halfSize);
                    posEnd.Position   = vec3(0.0f, axis, halfSize);
                    negStart.Position = vec3(0.0f,-axis,-halfSize);
                    negEnd.Position   = vec3(0.0f,-axis, halfSize);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    // Z
                    posStart.Position = vec3(0.0f,-halfSize, axis);
                    posEnd.Position   = vec3(0.0f, halfSize, axis);
                    negStart.Position = vec3(0.0f,-halfSize,-axis);
                    negEnd.Position   = vec3(0.0f, halfSize,-axis);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    break;
                case XY:
                    // X
                    posStart.Position = vec3( axis,-halfSize, 0.0f);
                    posEnd.Position   = vec3( axis, halfSize, 0.0f);
                    negStart.Position = vec3(-axis,-halfSize, 0.0f);
                    negEnd.Position   = vec3(-axis, halfSize, 0.0f);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    // Y
                    posStart.Position = vec3(-halfSize, axis, 0.0f);
                    posEnd.Position   = vec3( halfSize, axis, 0.0f);
                    negStart.Position = vec3(-halfSize,-axis, 0.0f);
                    negEnd.Position   = vec3( halfSize,-axis, 0.0f);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    break;
            }
        }
    }

    void
    GridDrawer::InitMinorGridData
    ()
    {
        debug("Grid: {}",__FUNCTION__);
        float halfSize = (mSize/2.0f);

        // Major Grid
        for (float axis = mMinorSpacing; axis <= halfSize; axis += mMinorSpacing)
        {
            GLWidgetVertex posStart, posEnd, negStart, negEnd;
            posStart.Color = mMinorColour;
            posEnd.Color   = mMinorColour;
            negStart.Color = mMinorColour;
            negEnd.Color   = mMinorColour;

            switch (mAxisPair)
            {
                case XZ:
                    // X
                    posStart.Position = vec3( axis, 0.0f,-halfSize);
                    posEnd.Position   = vec3( axis, 0.0f, halfSize);
                    negStart.Position = vec3(-axis, 0.0f,-halfSize);
                    negEnd.Position   = vec3(-axis, 0.0f, halfSize);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    // Z
                    posStart.Position = vec3(-halfSize, 0.0f, axis);
                    posEnd.Position   = vec3( halfSize, 0.0f, axis);
                    negStart.Position = vec3(-halfSize, 0.0f,-axis);
                    negEnd.Position   = vec3( halfSize, 0.0f,-axis);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    break;
                case YZ:
                    // Y
                    posStart.Position = vec3(0.0f, axis,-halfSize);
                    posEnd.Position   = vec3(0.0f, axis, halfSize);
                    negStart.Position = vec3(0.0f,-axis,-halfSize);
                    negEnd.Position   = vec3(0.0f,-axis, halfSize);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    // Z
                    posStart.Position = vec3(0.0f,-halfSize, axis);
                    posEnd.Position   = vec3(0.0f, halfSize, axis);
                    negStart.Position = vec3(0.0f,-halfSize,-axis);
                    negEnd.Position   = vec3(0.0f, halfSize,-axis);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    break;
                case XY:
                    // X
                    posStart.Position = vec3( axis,-halfSize, 0.0f);
                    posEnd.Position   = vec3( axis, halfSize, 0.0f);
                    negStart.Position = vec3(-axis,-halfSize, 0.0f);
                    negEnd.Position   = vec3(-axis, halfSize, 0.0f);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    // Y
                    posStart.Position = vec3(-halfSize, axis, 0.0f);
                    posEnd.Position   = vec3( halfSize, axis, 0.0f);
                    negStart.Position = vec3(-halfSize,-axis, 0.0f);
                    negEnd.Position   = vec3( halfSize,-axis, 0.0f);
                    AddLineVertex(posStart);
                    AddLineVertex(posEnd);
                    AddLineVertex(negStart);
                    AddLineVertex(negEnd);
                    break;
            }
        }
    }

    void GridDrawer::InitAxisLines()
    {
        debug("Grid: {}",__FUNCTION__);
        vec3 red(1,0,0);
        vec3 green(0,1,0);
        vec3 blue(0,0,1);

        GLWidgetVertex xStart, xEnd;
        xStart.Position = vec3(0);
        xStart.Color = red;
        xEnd.Position = vec3(mSize/2,0,0);
        xEnd.Color = red;
        AddLineVertex(xStart);
        AddLineVertex(xEnd);

        GLWidgetVertex yStart, yEnd;
        yStart.Position = vec3(0);
        yStart.Color = green;
        yEnd.Position = vec3(0,mSize/2,0);
        yEnd.Color = green;
        AddLineVertex(yStart);
        AddLineVertex(yEnd);

        GLWidgetVertex zStart, zEnd;
        zStart.Position = vec3(0);
        zStart.Color = blue;
        zEnd.Position = vec3(0,0,mSize/2);
        zEnd.Color = blue;
        AddLineVertex(zStart);
        AddLineVertex(zEnd);

        GLWidgetVertex ax1HomeStart, ax1HomeEnd, ax2HomeStart, ax2HomeEnd;
        ax1HomeStart.Color = mMajorColour;
        ax1HomeEnd.Color   = mMajorColour;
        ax2HomeStart.Color = mMajorColour;
        ax2HomeEnd.Color   = mMajorColour;

        switch (mAxisPair)
        {
            case XZ:
                ax1HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax1HomeEnd.Position   = vec3(-(mSize/2.0f),0.0f,0.0f);
                ax2HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax2HomeEnd.Position   = vec3(0.0f,0.0f,-(mSize/2.0f));
                break;
            case YZ:
                ax1HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax1HomeEnd.Position   = vec3(0.0f,-(mSize/2.0f),0.0f);
                ax2HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax2HomeEnd.Position   = vec3(0.0f,0.0f,-(mSize/2.0f));
                break;
            case XY:
                ax1HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax1HomeEnd.Position   = vec3(-(mSize/2.0f),0.0f,0.0f);
                ax2HomeStart.Position = vec3(0.0f,0.0f,0.0f);
                ax2HomeEnd.Position   = vec3(0.0f,-(mSize/2.0f),0.0f);
                break;

        }

        AddLineVertex(ax1HomeStart);
        AddLineVertex(ax1HomeEnd);
        AddLineVertex(ax2HomeStart);
        AddLineVertex(ax2HomeEnd);
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
        InitAxisLines();
        InitMajorGridData();
        InitMinorGridData();
        SubmitLineVertexBuffer();
    }
}
