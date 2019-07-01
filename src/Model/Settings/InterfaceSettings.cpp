/*
 * Copyright 2018 Octronic. All rights reserved.
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

#include "InterfaceSettings.h"
#include "../../Common/Logger.h"
#include <glm/gtc/type_ptr.hpp>

using glm::value_ptr;

namespace Coconut
{
	InterfaceSettings::InterfaceSettings()
		: mLineWidth(0.0f),
		  mFpsLock(60),
		  mAntiAliasing(true),
		  mVsync(true),
		  mMsaa(true),
		  mZbuffer(true),
		  mShowUiCommands(false),
		  mVisualiserColor(0),
		  mBackgroundColor(0),
		  mToolColor(0),
		  mToolPathColor(0),
		  mNormalColor(0),
		  mHighlightColor(0),
		  mStartPointColor(0),
		  mEndPointColor(0),
		  mTextColor(0),
		  mDrawnColor(0),
		  mZMovementColor(0)

	{
		debug("InterfaceSettings: Constructing");
	}

	float InterfaceSettings::GetLineWidth() const
	{
		return mLineWidth;
	}

	float* InterfaceSettings::GetLineWidthPtr()
	{
        return &mLineWidth;
    }

	void InterfaceSettings::SetLineWidth(float lineWidth)
	{
		mLineWidth = lineWidth;
	}

	int InterfaceSettings::GetFpsLock() const
	{
		return mFpsLock;
	}

	void InterfaceSettings::SetFpsLock(int fpsLock)
	{
		mFpsLock = fpsLock;
	}

	bool InterfaceSettings::GetShowUiCommands() const
	{
		return mShowUiCommands;
	}

	void InterfaceSettings::SetShowUiCommands(bool show)
	{
		mShowUiCommands = show;
	}

	vec4 InterfaceSettings::GetVisualiserColor() const
	{
        return mVisualiserColor;
    }

    float* InterfaceSettings::GetVisualiserColorArray()
    {
    	return value_ptr(mVisualiserColor);
    }

	void InterfaceSettings::SetVisualiserColor(const vec4& visualiserColor)
	{
		mVisualiserColor = visualiserColor;
	}

	vec4 InterfaceSettings::GetBackgroundColor() const
	{
        return mBackgroundColor;
    }

    float* InterfaceSettings::GetBackgroundColorArray()
    {
       return value_ptr(mBackgroundColor);
    }

	void InterfaceSettings::SetBackgroundColor(const vec4& backgroundColor)
	{
		mBackgroundColor = backgroundColor;
	}

	vec4 InterfaceSettings::GetToolColor() const
	{
        return mToolColor;
    }

    float* InterfaceSettings::GetToolColorArray()
    {
       return value_ptr(mToolColor);
    }

	void InterfaceSettings::SetToolColor(const vec4& toolColor)
	{
		mToolColor = toolColor;
	}

	vec4 InterfaceSettings::GetToolPathColor() const
	{
        return mToolPathColor;
    }

    float* InterfaceSettings::GetToolPathColorArray()
    {

       return value_ptr(mToolPathColor);
    }

	void InterfaceSettings::SetToolPathColor(const vec4& toolPathColor)
	{
		mToolPathColor = toolPathColor;
	}

	vec4 InterfaceSettings::GetNormalColor() const
	{
        return mNormalColor;
    }

    float* InterfaceSettings::GetNormalColorArray()
    {
       return value_ptr(mNormalColor);
    }

	void InterfaceSettings::SetNormalColor(const vec4& normalColor)
	{
		mNormalColor = normalColor;
	}

	vec4 InterfaceSettings::GetHighlightColor() const
	{
        return mHighlightColor;
    }

    float* InterfaceSettings::GetHighlightColorArray()
    {
       return value_ptr(mHighlightColor);
    }

	void InterfaceSettings::SetHighlightColor(const vec4& highlightColor)
	{
		mHighlightColor = highlightColor;
	}

	vec4 InterfaceSettings::GetStartPointColor() const
	{
        return mStartPointColor;
    }

    float* InterfaceSettings::GetStartPointColorArray()
    {
       return value_ptr(mStartPointColor);
    }

	void InterfaceSettings::SetStartPointColor(const vec4& startPointColor)
	{
		mStartPointColor = startPointColor;
	}

	vec4 InterfaceSettings::GetEndPointColor() const
	{
        return mEndPointColor;
    }

    float* InterfaceSettings::GetEndPointColorArray()
    {
       return value_ptr(mEndPointColor);
    }

	void InterfaceSettings::SetEndPointColor(const vec4& endPointColor)
	{
		mEndPointColor = endPointColor;
	}

	vec4 InterfaceSettings::GetTextColor() const
	{
        return mTextColor;
    }

    float* InterfaceSettings::GetTextColorArray()
    {
       return value_ptr(mTextColor);
    }

	void InterfaceSettings::SetTextColor(const vec4& textColor)
	{
		mTextColor = textColor;
	}

	vec4 InterfaceSettings::GetDrawnColor() const
	{
        return mDrawnColor;
    }

    float* InterfaceSettings::GetDrawnColorArray()
    {
       return value_ptr(mDrawnColor);
    }

	void InterfaceSettings::SetDrawnColor(const vec4& drawnColor)
	{
		mDrawnColor = drawnColor;
	}

	vec4 InterfaceSettings::GetZMovementColor() const
	{
        return mZMovementColor;
    }

    float* InterfaceSettings::GetZMovementColorArray()
    {

       return value_ptr(mZMovementColor);
    }

	void InterfaceSettings::SetZMovementColor(const vec4& zMovementColor)
	{
        mZMovementColor = zMovementColor;
    }

    nlohmann::json InterfaceSettings::ToJson()
    {
       json j;
       return j;
    }

    bool InterfaceSettings::FromJson(const nlohmann::json& j)
    {
       return false;
    }

	// Graphics
	const string InterfaceSettings::GFX = "Graphics";
	const string InterfaceSettings::GFX_ANTIALIASING = "AntiAliasing";
	const string InterfaceSettings::GFX_MSAA = "MSAA";
	const string InterfaceSettings::GFX_VSYNC = "VSync";
	const string InterfaceSettings::GFX_ZBUFFER = "ZBuffer";
	const string InterfaceSettings::GFX_SIMPLIFY = "Simplify";
	const string InterfaceSettings::GFX_SIMPLIFY_PRECISION = "SimplifyPrecision";
	const string InterfaceSettings::GFX_GRAYSCALE = "Grayscale";
	const string InterfaceSettings::GFX_GRAYSCALE_S_CODE = "GrayscaleSCode";
	const string InterfaceSettings::GFX_GRAYSCALE_Z_CODE = "GrayscaleZCode";
	const string InterfaceSettings::GFX_DRAW_MODE_VECTORS = "DrawModeVectors";
	// Ui
	const string InterfaceSettings::UI = "Ui";
	const string InterfaceSettings::UI_JOG_STEP = "JogStep";
	const string InterfaceSettings::UI_SPINDLE_SPEED = "SpindleSpeed";
	const string InterfaceSettings::UI_MOVE_ON_RESTORE = "MoveOnRestore";
	const string InterfaceSettings::UI_RESTORE_MODE = "RestoreMode";
	const string InterfaceSettings::UI_SHOW_PROGRAM_COMMANDS = "ShowProgramCommands";
	const string InterfaceSettings::UI_CONSOLE_SHOW_UI_CMDS = "ShowUiCommandsInConsole";
	const string InterfaceSettings::UI_SPINDLE_SPEED_MIN = "SpindleSpeedMin";
	const string InterfaceSettings::UI_SPINDLE_SPEED_MAX =  "SpindleSpeedMax";
	const string InterfaceSettings::UI_PANEL_SHOW_SPINDLE = "PandlShowSpindle";
	const string InterfaceSettings::UI_PANEL_SHOW_FEED = "PanelShowFeed";
	const string InterfaceSettings::UI_PANEL_SHOW_JOG = "PanelShowJog";
	const string InterfaceSettings::UI_AUTOSCROLL_CHECKED = "AutoscrollChecked";
	const string InterfaceSettings::UI_UNITS = "Uints";
	const string InterfaceSettings::UI_LAST_FOLDER = "LastFolder";
	const string InterfaceSettings::UI_KEYBOARD_CONTROL= "KeyboardControl";
	const string InterfaceSettings::UI_AUTO_COMPLETION = "AutoCompletion";
	const string InterfaceSettings::UI_TOUCH_COMMAND = "TouchCommand";
	const string InterfaceSettings::UI_SAFE_POSITION = "SafePosition";
	const string InterfaceSettings::UI_COMMAND_ITEMS = "CommandItems";
	const string InterfaceSettings::UI_COMMAND_INDEX = "CommandIndex";
	const string InterfaceSettings::UI_FEED_OVERRIDE = "FeedOverride";
	// Visualisser
	const string InterfaceSettings::VISUALISER = "Visualiser";
	const string InterfaceSettings::VISUALISER_LINE_WIDTH = "LineWidth";
	const string InterfaceSettings::VISUALISER_ARC_LENGTH = "ArcLength";
	const string InterfaceSettings::VISUALISER_ARC_DEGREE = "ArcDegree";
	const string InterfaceSettings::VISUALISER_ARC_DEGREE_MODE = "ArcDegreeMode";
	const string InterfaceSettings::VISUALISER_RAPID_SPEED = "RapidSpeed";
	const string InterfaceSettings::VISUALISER_ACCELERATION = "Acceleration";
	const string InterfaceSettings::VISUALISER_FPS = "FPS";
	const string InterfaceSettings::VISUALISER_QUERY_STATE_TIME = "QueryStateTime";
}
