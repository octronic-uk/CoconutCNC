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

#include "../../AppState.h"
#include "../../Common/Logger.h"
#include "../../Model/Settings/SettingsModel.h"
#include "../../Model/Settings/MachineSettings.h"

namespace Coconut
{

    GridDrawer::GridDrawer(AppState* p,
		float majorSpacing, float minorSpacing,
		vec3 majorColour, vec3 minorColour)
        : GLWidget(p, "Grid"),
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
        MachineSettings& ms = mAppState->GetSettingsModel().GetMachineSettings();
        GLWidgetVertex lineStart, lineEnd;
		lineStart.Color = mMajorColour;
		lineEnd.Color = mMajorColour;

        vec3 work_area = ms.GetWorkArea();

		// X
        for (float step = 0; step <= work_area.x; step += mMajorSpacing)
        {
			lineStart.Position = vec3(step, 0.0f, 0);
			lineEnd.Position   = vec3(step, work_area.x, 0.0f);
			AddLineVertex(lineStart);
			AddLineVertex(lineEnd);
        }

        // Y
        for (float step = 0; step <= work_area.y; step += mMajorSpacing)
        {
			lineStart.Position = vec3(0, step, 0.0f);
			lineEnd.Position   = vec3(work_area.y, step, 0.0f);
			AddLineVertex(lineStart);
			AddLineVertex(lineEnd);
        }
    }

    void
    GridDrawer::InitMinorGridData
    ()
    {
        debug("Grid: {}",__FUNCTION__);

        MachineSettings& ms = mAppState->GetSettingsModel().GetMachineSettings();
        GLWidgetVertex lineStart, lineEnd;
		lineStart.Color = mMinorColour;
		lineEnd.Color = mMinorColour;

        vec3 work_area = ms.GetWorkArea();

		// X
        for (float step = 0; step <= work_area.x; step += mMinorSpacing)
        {
            if (fmod(step,mMajorSpacing) == 0.0f) continue;
			lineStart.Position = vec3(step, 0.0f, 0);
			lineEnd.Position   = vec3(step, work_area.x, 0.0f);
			AddLineVertex(lineStart);
			AddLineVertex(lineEnd);
        }

        // Y
        for (float step = 0; step <= work_area.y; step += mMinorSpacing)
        {
            if (fmod(step,mMajorSpacing) == 0.0f) continue;
			lineStart.Position = vec3(0, step, 0.0f);
			lineEnd.Position   = vec3(work_area.y, step, 0.0f);
			AddLineVertex(lineStart);
			AddLineVertex(lineEnd);
        }
	}

    vec3 GridDrawer::GetMinorColour() const
    {
        return mMinorColour;
    }

    void GridDrawer::SetMinorColour(vec3 minorColour)
    {
        mMinorColour = minorColour;
    }

    vec3 GridDrawer::GetMajorColour() const
    {
        return mMajorColour;
    }

    void GridDrawer::SetMajorColour(vec3 majorColour)
    {
        mMajorColour = majorColour;
    }

    void GridDrawer::SetTranslation(vec3 translation)
    {
        mModelMatrix = glm::translate(mat4(1.0f),translation);
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
