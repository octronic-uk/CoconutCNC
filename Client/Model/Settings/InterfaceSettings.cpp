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
#include "../../Logger.h"

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
		  mVisualiserColor(""),
		  mBackgroundColor(""),
		  mToolColor(""),
		  mToolPathColor(""),
		  mNormalColor(""),
		  mHighlightColor(""),
		  mStartPointColor(""),
		  mEndPointColor(""),
		  mTextColor(""),
		  mDrawnColor(""),
		  mZMovementColor("")

	{
		debug("InterfaceSettings: Constructing");
	}

	float InterfaceSettings::GetLineWidth() const
	{
		return mLineWidth;
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

	bool InterfaceSettings::GetAntiAliasing() const
	{
		return mAntiAliasing;
	}

	void InterfaceSettings::SetAntiAliasing(bool antiAliasing)
	{
		mAntiAliasing = antiAliasing;
	}

	bool InterfaceSettings::GetVsync() const
	{
		return mVsync;
	}

	void InterfaceSettings::SetVsync(bool vsync)
	{
		mVsync = vsync;
	}

	bool InterfaceSettings::GetMsaa() const
	{
		return mMsaa;
	}

	void InterfaceSettings::SetMsaa(bool msaa)
	{
		mMsaa = msaa;
	}

	bool InterfaceSettings::GetZbuffer() const
	{
		return mZbuffer;
	}

	void InterfaceSettings::SetZbuffer(bool zbuffer)
	{
		mZbuffer = zbuffer;
	}

	bool InterfaceSettings::GetShowUiCommands() const
	{
		return mShowUiCommands;
	}

	void InterfaceSettings::SetShowUiCommands(bool show)
	{
		mShowUiCommands = show;
	}

	string InterfaceSettings::GetVisualiserColor() const
	{
		return mVisualiserColor;
	}

	void InterfaceSettings::SetVisualiserColor(const string& visualiserColor)
	{
		mVisualiserColor = visualiserColor;
	}

	string InterfaceSettings::GetBackgroundColor() const
	{
		return mBackgroundColor;
	}

	void InterfaceSettings::SetBackgroundColor(const string& backgroundColor)
	{
		mBackgroundColor = backgroundColor;
	}

	string InterfaceSettings::GetToolColor() const
	{
		return mToolColor;
	}

	void InterfaceSettings::SetToolColor(const string& toolColor)
	{
		mToolColor = toolColor;
	}

	string InterfaceSettings::GetToolPathColor() const
	{
		return mToolPathColor;
	}

	void InterfaceSettings::SetToolPathColor(const string& toolPathColor)
	{
		mToolPathColor = toolPathColor;
	}

	string InterfaceSettings::GetNormalColor() const
	{
		return mNormalColor;
	}

	void InterfaceSettings::SetNormalColor(const string& normalColor)
	{
		mNormalColor = normalColor;
	}

	string InterfaceSettings::GetHighlightColor() const
	{
		return mHighlightColor;
	}

	void InterfaceSettings::SetHighlightColor(const string& highlightColor)
	{
		mHighlightColor = highlightColor;
	}

	string InterfaceSettings::GetStartPointColor() const
	{
		return mStartPointColor;
	}

	void InterfaceSettings::SetStartPointColor(const string& startPointColor)
	{
		mStartPointColor = startPointColor;
	}

	string InterfaceSettings::GetEndPointColor() const
	{
		return mEndPointColor;
	}

	void InterfaceSettings::SetEndPointColor(const string& endPointColor)
	{
		mEndPointColor = endPointColor;
	}

	string InterfaceSettings::GetTextColor() const
	{
		return mTextColor;
	}

	void InterfaceSettings::SetTextColor(const string& textColor)
	{
		mTextColor = textColor;
	}

	string InterfaceSettings::GetDrawnColor() const
	{
		return mDrawnColor;
	}

	void InterfaceSettings::SetDrawnColor(const string& drawnColor)
	{
		mDrawnColor = drawnColor;
	}

	string InterfaceSettings::GetZMovementColor() const
	{
		return mZMovementColor;
	}

	void InterfaceSettings::SetZMovementColor(const string& zMovementColor)
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
