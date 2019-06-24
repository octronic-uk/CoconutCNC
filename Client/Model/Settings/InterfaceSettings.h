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

#pragma once

#include <string>
#include "JsonSerialization.h"

using std::string;

namespace Coconut
{
	class InterfaceSettings : public JsonSerialization
	{
	public:
		InterfaceSettings();

		float GetLineWidth() const;
		void SetLineWidth(float lineWidth);

		int GetFpsLock() const;
		void SetFpsLock(int fpsLock);

		bool GetAntiAliasing() const;
		void SetAntiAliasing(bool antiAliasing);

		bool GetVsync() const;
		void SetVsync(bool vsync);

		bool GetMsaa() const;
		void SetMsaa(bool msaa);

		bool GetZbuffer() const;
		void SetZbuffer(bool zbuffer);

		bool GetShowUiCommands() const;
		void SetShowUiCommands(bool showUiCommands);

		bool GetSimplifyGeometry() const;
		void SetSimplifyGeometry(bool simplifyGeometry);

		float GetSimplifyPrecision() const;
		void SetSimplifyPrecision(float simplifyPrecision);

		bool GetGrayscaleSegments() const;
		void SetGrayscaleSegments(bool grayscaleSegments);

		bool GetGrayscaleCode() const;
		void SetGrayscaleCode(bool grayscaleCode);

		string GetVisualiserColor() const;
		void SetVisualiserColor(const string& visualiserColor);

		string GetBackgroundColor() const;
		void SetBackgroundColor(const string& backgroundColor);

		string GetToolColor() const;
		void SetToolColor(const string& toolColor);

		string GetToolPathColor() const;
		void SetToolPathColor(const string& toolPathColor);

		string GetNormalColor() const;
		void SetNormalColor(const string& normalColor);

		string GetHighlightColor() const;
		void SetHighlightColor(const string& highlightColor);

		string GetStartPointColor() const;
		void SetStartPointColor(const string& startPointColor);

		string GetEndPointColor() const;
		void SetEndPointColor(const string& endPointColor);

		string GetTextColor() const;
		void SetTextColor(const string& textColor);

		string GetDrawnColor() const;
		void SetDrawnColor(const string& drawnColor);

		string GetZMovementColor() const;
		void SetZMovementColor(const string& zMovementColor);

        // JsonSerialization interface
        json ToJson() override;
        bool FromJson(const json& j) override;

	private:
		float mLineWidth;
		int mFpsLock;
		bool mAntiAliasing;
		bool mVsync;
		bool mMsaa;
		bool mZbuffer;
		bool mShowUiCommands;
		string mVisualiserColor;
		string mBackgroundColor;
		string mToolColor;
		string mToolPathColor;
		string mNormalColor;
		string mHighlightColor;
		string mStartPointColor;
		string mEndPointColor;
		string mTextColor;
		string mDrawnColor;
		string mZMovementColor;

        // Graphics
		const static string GFX;
		const static string GFX_ANTIALIASING;
		const static string GFX_MSAA;
		const static string GFX_VSYNC;
		const static string GFX_ZBUFFER;
		const static string GFX_SIMPLIFY;
		const static string GFX_SIMPLIFY_PRECISION;
		const static string GFX_GRAYSCALE;
		const static string GFX_GRAYSCALE_S_CODE;
		const static string GFX_GRAYSCALE_Z_CODE;
		const static string GFX_DRAW_MODE_VECTORS;

		// Ui
		const static string UI;
		const static string UI_JOG_STEP;
		const static string UI_SPINDLE_SPEED;
		const static string UI_MOVE_ON_RESTORE;
		const static string UI_RESTORE_MODE;
		const static string UI_SHOW_PROGRAM_COMMANDS;
		const static string UI_CONSOLE_SHOW_UI_CMDS;
		const static string UI_SPINDLE_SPEED_MIN;
		const static string UI_SPINDLE_SPEED_MAX;
		const static string UI_PANEL_SHOW_SPINDLE;
		const static string UI_PANEL_SHOW_FEED;
		const static string UI_PANEL_SHOW_JOG;
		const static string UI_AUTOSCROLL_CHECKED;
		const static string UI_UNITS;
		const static string UI_LAST_FOLDER;
		const static string UI_KEYBOARD_CONTROL;
		const static string UI_AUTO_COMPLETION;
		const static string UI_TOUCH_COMMAND;
		const static string UI_SAFE_POSITION;
		const static string UI_COMMAND_ITEMS;
		const static string UI_COMMAND_INDEX;
		const static string UI_FEED_OVERRIDE;

		// Visualisser
		const static string VISUALISER;
		const static string VISUALISER_LINE_WIDTH;
		const static string VISUALISER_ARC_LENGTH;
		const static string VISUALISER_ARC_DEGREE;
		const static string VISUALISER_ARC_DEGREE_MODE;
		const static string VISUALISER_RAPID_SPEED;
		const static string VISUALISER_ACCELERATION;
		const static string VISUALISER_FPS;
		const static string VISUALISER_QUERY_STATE_TIME;

	};
}
