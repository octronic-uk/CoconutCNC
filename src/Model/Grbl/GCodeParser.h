// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeParser.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <glm/vec3.hpp>
#include <vector>
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

		double GetSmallArcSegmentLength();
		void SetSmallArcSegmentLength(double smallArcSegmentLength);

		double GetSmallArcThreshold();
		void SetSmallArcThreshold(double smallArcThreshold);

		double GetSpeedOverride();
		void SetSpeedOverride(double speedOverride);

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
		double GetTraverseSpeed() const;
		void SetTraverseSpeed(double traverseSpeed);
		int GetCommandNumber() const;

		static string OverrideSpeed(const string& command, double speed);
		static string RemoveComment(const string& command);
		static string ParseComment(const string& command);
		static string TruncateDecimals(int length, const string& command);
		static string RemoveAllWhitespace(const string& command);
		static vector<float> ParseCodes(const vector<string> &args, char code);
		static vector<int> ParseGCodes(const string& command);
		static vector<int> ParseMCodes(const string& command);
		static vector<string> SplitCommand(const string& command);
		static double ParseCoord(const vector<string>& argList, char c);
		static vec3 UpdatePointWithCommand
        (const vec3& initial, double x, double y, double z, bool absoluteMode);
		static vec3 UpdatePointWithCommand
        (const vector<string>& commandArgs, const vec3 &initial, bool absoluteMode);
		static vec3 UpdatePointWithCommand
        (const string& command, const vec3 &initial, bool absoluteMode);
		static vec3 ConvertRToCenter
        (const vec3& start, const vec3& end, double radius, bool absoluteIJK, bool clockwise);
		static vec3 UpdateCenterWithCommand
        (const vector<string>& commandArgs, const vec3& initial, const vec3& nextPoint, bool absoluteIJKMode, bool clockwise);
		static string GenerateG1FromPoints
        (const vec3& start, const vec3& end, bool absoluteMode, int precision);
		static double GetAngle(const vec3& start, const vec3& end);
		static double CalculateSweep(double startAngle, double endAngle, bool isCw);
		static vector<vec3> GeneratePointsAlongArcBDring
        (PointSegment::planes plane, vec3& start, vec3& end, vec3& center, bool clockwise, double R, double minArcLength, double arcPrecision, bool arcDegreeMode);
		static vector<vec3> GeneratePointsAlongArcBDring
        (PointSegment::planes plane, vec3& p1, vec3& p2, vec3& center, bool isCw, double radius, double startAngle, double sweep, int numPoints);
		static bool IsDigit(char c);
		static bool IsLetter(char c);
		static char ToUpper(char c);
		static GCodeCommand OverrideSpeed(const GCodeCommand& command, double speed);

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
		double mSpeedOverride;
		int    mTruncateDecimalLength;
		bool   mRemoveAllWhitespace;
		bool   mConvertArcsToLines;
		double mSmallArcThreshold;

		// Not configurable outside, but maybe it should be.
		double mSmallArcSegmentLength;
		double mLastSpeed;
		double mTraverseSpeed;
		double mLastSpindleSpeed;


	};
}
