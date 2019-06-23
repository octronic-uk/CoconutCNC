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
	class GcodeParser
	{
	public:
		GcodeParser();
		~GcodeParser();

		void initialise();

		bool getConvertArcsToLines();
		void setConvertArcsToLines(bool convertArcsToLines);

		bool getRemoveAllWhitespace();
		void setRemoveAllWhitespace(bool removeAllWhitespace);

		double getSmallArcSegmentLength();
		void setSmallArcSegmentLength(double smallArcSegmentLength);

		double getSmallArcThreshold();
		void setSmallArcThreshold(double smallArcThreshold);

		double getSpeedOverride();
		void setSpeedOverride(double speedOverride);

		int getTruncateDecimalLength();
		void setTruncateDecimalLength(int truncateDecimalLength);

		void reset(const vec3& initialPoint);

		PointSegment* addCommandFromArgs(GCodeCommand* command);
		PointSegment* addCommand(GCodeCommand* command);

		vec3 getCurrentPoint();

		vector<PointSegment*> expandArc(GCodeCommand*);
		//vector<string> preprocessCommands(vector<string> commands);
		//vector<string> preprocessCommand(string command);
		//vector<string> convertArcsToLines(string command);

		vector<PointSegment*> getPointSegmentHandlesList();
		double getTraverseSpeed() const;
		void setTraverseSpeed(double traverseSpeed);
		int getCommandNumber() const;

		static string overrideSpeed(const string command, double speed);
		static string removeComment(string command);
		static string parseComment(const string command);
		static string truncateDecimals(int length, string command);
		static string removeAllWhitespace(const string command);
		static vector<float> parseCodes(const vector<string> &args, char code);
		static vector<int> parseGCodes(const string command);
		static vector<int> parseMCodes(const string command);
		static vector<string> splitCommand(const string &command);
		static double parseCoord(vector<string> argList, char c);
		static vec3 updatePointWithCommand(const vec3 &initial, double x, double y, double z, bool absoluteMode);
		static vec3 updatePointWithCommand(const vector<string> &commandArgs, const vec3 &initial, bool absoluteMode);
		static vec3 updatePointWithCommand(const string &command, const vec3 &initial, bool absoluteMode);
		static vec3 convertRToCenter(vec3 start, vec3 end, double radius, bool absoluteIJK, bool clockwise);
		static vec3 updateCenterWithCommand(vector<string> commandArgs, vec3 initial, vec3 nextPoint, bool absoluteIJKMode, bool clockwise);
		static string generateG1FromPoints(vec3 start, vec3 end, bool absoluteMode, int precision);
		static double getAngle(vec3 start, vec3 end);
		static double calculateSweep(double startAngle, double endAngle, bool isCw);
		static vector<vec3> generatePointsAlongArcBDring(PointSegment::planes plane, vec3 start, vec3 end, vec3 center, bool clockwise, double R, double minArcLength, double arcPrecision, bool arcDegreeMode);
		static vector<vec3> generatePointsAlongArcBDring(PointSegment::planes plane, vec3 p1, vec3 p2, vec3 center, bool isCw, double radius, double startAngle, double sweep, int numPoints);
		static bool isDigit(char c);
		static bool isLetter(char c);
		static char toUpper(char c);
		static GCodeCommand* overrideSpeed(const GCodeCommand *command, double speed);

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

	private:
		// Current state
		bool mIsMetric;
		bool mInAbsoluteMode;
		bool mInAbsoluteIJKMode;
		float mLastGcodeCommand;
		vec3 mCurrentPoint;
		int mCommandNumber;
		PointSegment::planes mCurrentPlane;

		// Settings
		double mSpeedOverride;
		int mTruncateDecimalLength;
		bool mRemoveAllWhitespace;
		bool mConvertArcsToLines;
		double mSmallArcThreshold;
		// Not configurable outside, but maybe it should be.
		double mSmallArcSegmentLength;

		double mLastSpeed;
		double mTraverseSpeed;
		double mLastSpindleSpeed;

		// The gcode.
		vector<PointSegment*> mPoints;
		PointSegment* processCommand(GCodeCommand* command);
		void handleMCode(float code, const vector<string> &args);
		PointSegment* handleGCode(GCodeCommand* cmd, float code);
		PointSegment* addLinearPointSegment(GCodeCommand* cmd, const vec3 &nextPoint, bool fastTraverse);
		PointSegment* addArcPointSegment(GCodeCommand* cmd, const vec3 &nextPoint, bool clockwise, const vector<string> &args);
		void setLastGcodeCommand(float num);
	};
}
