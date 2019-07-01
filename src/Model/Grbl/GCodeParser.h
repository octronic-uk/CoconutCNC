// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeParser.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <glm/vec3.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include "PointSegment.h"
#include "GCodeCommand.h"

using glm::vec3;
using std::vector;

namespace Coconut
{
	class GCodeParser
	{
	public:
		GCodeParser();
		~GCodeParser();

		void ClearState();

		bool GetConvertArcsToLines();
		void SetConvertArcsToLines(bool convertArcsToLines);

		bool GetRemoveAllWhitespace();
		void SetRemoveAllWhitespace(bool removeAllWhitespace);

		float GetSmallArcSegmentLength();
		void SetSmallArcSegmentLength(float smallArcSegmentLength);

		float GetSmallArcThreshold();
		void SetSmallArcThreshold(float smallArcThreshold);

		float GetSpeedOverride();
		void SetSpeedOverride(float speedOverride);

		int GetTruncateDecimalLength();
		void SetTruncateDecimalLength(int truncateDecimalLength);

		void Reset(const vec3& initialPoint);

		PointSegment AddCommandFromArgs(const GCodeCommand& command);
		PointSegment AddCommand(GCodeCommand& command);

		vec3 GetCurrentPoint();

		vector<PointSegment> ExpandArc(const GCodeCommand& cmd);
		//vector<string> preprocessCommands(vector<string> commands);
		//vector<string> preprocessCommand(string command);
		//vector<string> convertArcsToLines(string command);

		vector<PointSegment>& GetPointSegmentList();
		float GetTraverseSpeed() const;
		void SetTraverseSpeed(float traverseSpeed);
		int GetCommandNumber() const;

		static string OverrideSpeed(const string& command, float speed);
		static string RemoveComment(const string& command);
		static string ParseComment(const string& command);
		static string TruncateDecimals(int length, const string& command);
		static string RemoveAllWhitespace(const string& command);
		static vector<float> ParseCodes(const vector<string> &args, char code);
		static vector<int> ParseGCodes(const string& command);
		static vector<int> ParseMCodes(const string& command);
		static vector<string> SplitCommand(const string& command);
		static float ParseCoord(const vector<string>& argList, char c);
		static vec3 UpdatePointWithCommand
        (const vec3& initial, float x, float y, float z, bool absoluteMode);
		static vec3 UpdatePointWithCommand
        (const vector<string>& commandArgs, const vec3 &initial, bool absoluteMode);
		static vec3 UpdatePointWithCommand
        (const string& command, const vec3 &initial, bool absoluteMode);
		static vec3 ConvertRToCenter
        (const vec3& start, const vec3& end, float radius, bool absoluteIJK, bool clockwise);
		static vec3 UpdateCenterWithCommand
        (const vector<string>& commandArgs, const vec3& initial, const vec3& nextPoint, bool absoluteIJKMode, bool clockwise);
		static string GenerateG1FromPoints
        (const vec3& start, const vec3& end, bool absoluteMode, int precision);
		static float GetAngle(const vec3& start, const vec3& end);
		static float CalculateSweep(float startAngle, float endAngle, bool isCw);
		static vector<vec3> GeneratePointsAlongArcBDring
        (PointSegment::planes plane, vec3& start, vec3& end, vec3& center, bool clockwise, float R, float minArcLength, float arcPrecision, bool arcDegreeMode);
		static vector<vec3> GeneratePointsAlongArcBDring
        (PointSegment::planes plane, vec3& p1, vec3& p2, vec3& center, bool isCw, float radius, float startAngle, float sweep, int numPoints);
		static bool IsDigit(char c);
		static bool IsLetter(char c);
		static char ToUpper(char c);
		static GCodeCommand OverrideSpeed(const GCodeCommand& command, float speed);

		const float GCODE_RAPID = 0.0f;
		const float GCODE_LINEAR_INTERPOLATION = 1.0;
		const float GCODE_STRAIGHT_PROBE = 38.2f;
		const float GCODE_ARC_MOVE_IJK = 2.0f;
		const float GCODE_ARC_MOVE_RP = 3.0f;
		const float GCODE_PLANE_XY = 17.0f;
		const float GCODE_PLANE_ZX = 18.0f;
		const float GCODE_PLANE_YZ = 19.0f;
		const float GCODE_UNITS_INCHES = 20.0f;
		const float GCODE_UNITS_MM = 21.0f;
		const float GCODE_DISTANCE_ABSOLUTE = 90.0f;
		const float GCODE_DISTANCE_ABSOLUTE_IJK = 90.1f;
		const float GCODE_DISTANCE_INCREMENTAL = 91.0f;
		const float GCODE_DISTANCE_INCREMENTAL_IJK = 91.1f;

    protected:
        PointSegment ProcessCommand(const GCodeCommand& command);
		void         HandleMCode(float code, const vector<string> &args);
		PointSegment HandleGCode(const GCodeCommand& cmd, float code);
		PointSegment AddLinearPointSegment(const GCodeCommand& cmd, const vec3& nextPoint, bool fastTraverse);
		PointSegment AddArcPointSegment(const GCodeCommand& cmd, const vec3& nextPoint, bool clockwise, const vector<string>& args);
		void         SetLastGcodeCommand(float num);

	private:
		// Current state
		bool  mIsMetric;
		bool  mInAbsoluteMode;
		bool  mInAbsoluteIJKMode;
		float mLastGcodeCommand;
		vec3  mCurrentPoint;
		int   mCommandNumber;
		PointSegment::planes mCurrentPlane;
		vector<PointSegment> mPoints; // The gcode.

		// Settings
		float mSpeedOverride;
		int    mTruncateDecimalLength;
		bool   mRemoveAllWhitespace;
		bool   mConvertArcsToLines;
		float mSmallArcThreshold;

		// Not configurable outside, but maybe it should be.
		float mSmallArcSegmentLength;
		float mLastSpeed;
		float mTraverseSpeed;
		float mLastSpindleSpeed;


	};
}
