// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeParser.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GCodeParser.h"

#include <iostream>
#include <cmath>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <regex>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../Common/Util.h"
#include "../../Common/Logger.h"

using std::smatch;
using std::ssub_match;
using std::regex;
using std::regex_search;
using glm::mat4;
using std::cout;
using std::endl;
using std::isnan;
using glm::rotate;
using std::stringstream;
using std::fixed;
using std::setprecision;
using std::exception;

namespace Coconut
{
	GCodeParser::GCodeParser()
		: mIsMetric(true),
		  mInAbsoluteMode(true),
		  mInAbsoluteIJKMode(false),
		  mLastGcodeCommand(-1),
		  mCurrentPoint(vec3(NAN)),
		  mCommandNumber(0),
		  mSpeedOverride(-1),
		  mTruncateDecimalLength(40),
		  mRemoveAllWhitespace(true),
		  mConvertArcsToLines(false),
		  mSmallArcThreshold(1.0),
		  mSmallArcSegmentLength(0.3),
		  mLastSpeed(0),
		  mTraverseSpeed(300),
		  mLastSpindleSpeed(0),
          mCurrentPlane(PointSegment::planes::XY)
	{
		info("GcodeParser: Constructing");
		Reset(vec3(NAN));
	}

	GCodeParser::~GCodeParser()
	{
		info("GcodeParser: Destructing");
		mPoints.clear();
		mIsMetric = true;
		mInAbsoluteMode = true;
		mInAbsoluteIJKMode = false;
		mLastGcodeCommand = -1;
		mCommandNumber = 0;
		mSpeedOverride = -1;
		mTruncateDecimalLength = 40;
		mRemoveAllWhitespace = true;
		mConvertArcsToLines = false;
		mSmallArcThreshold = 1.0;
		mSmallArcSegmentLength = 0.3;
		mLastSpeed = 0;
		mTraverseSpeed = 300;
		mLastSpindleSpeed = 0;
	}

    void GCodeParser::ClearState()
    {
        mPoints.clear();
		mIsMetric = true;
		mInAbsoluteMode = true;
		mInAbsoluteIJKMode = false;
		mLastGcodeCommand = -1;
		mCommandNumber = 0;
		mSpeedOverride = -1;
		mTruncateDecimalLength = 40;
		mRemoveAllWhitespace = true;
		mConvertArcsToLines = false;
		mSmallArcThreshold = 1.0;
		mSmallArcSegmentLength = 0.3;
		mLastSpeed = 0;
		mTraverseSpeed = 300;
		mLastSpindleSpeed = 0;
    }

	bool GCodeParser::GetConvertArcsToLines()
	{
		return mConvertArcsToLines;
	}

	void GCodeParser::SetConvertArcsToLines(bool convertArcsToLines)
	{
		mConvertArcsToLines = convertArcsToLines;
	}

	bool GCodeParser::GetRemoveAllWhitespace()
	{
		return mRemoveAllWhitespace;
	}

	void GCodeParser::SetRemoveAllWhitespace(bool removeAllWhitespace)
	{
		mRemoveAllWhitespace = removeAllWhitespace;
	}

	double GCodeParser::GetSmallArcSegmentLength()
	{
		return mSmallArcSegmentLength;
	}

	void GCodeParser::SetSmallArcSegmentLength(double smallArcSegmentLength)
	{
		mSmallArcSegmentLength = smallArcSegmentLength;
	}

	double GCodeParser::GetSmallArcThreshold()
	{
		return mSmallArcThreshold;
	}

	void GCodeParser::SetSmallArcThreshold(double smallArcThreshold)
	{
		mSmallArcThreshold = smallArcThreshold;
	}

	double GCodeParser::GetSpeedOverride()
	{
		return mSpeedOverride;
	}

	void GCodeParser::SetSpeedOverride(double speedOverride)
	{
		mSpeedOverride = speedOverride;
	}

	int GCodeParser::GetTruncateDecimalLength()
	{
		return mTruncateDecimalLength;
	}

	void GCodeParser::SetTruncateDecimalLength(int truncateDecimalLength)
	{
		mTruncateDecimalLength = truncateDecimalLength;
	}

	// ReSets the current state.
	void GCodeParser::Reset(const vec3& initialPoint)
	{
		info("GcodeParser: ReSeting Parser with initial point {},{},{}", initialPoint.x, initialPoint.y,initialPoint.z);
		mPoints.clear();
		// The unspoken home location.
		mCurrentPoint = initialPoint;
		mCurrentPlane = PointSegment::XY;
		mPoints.push_back(PointSegment(GCodeCommand::NoParent, mCurrentPoint, -1));
	}

	/**
	* Add a command to be processed.
	*/
	PointSegment GCodeParser::AddCommand(GCodeCommand& command)
	{
		string stripped = RemoveComment(command.GetCommand());
		vector<string> args = SplitCommand(stripped);
		command.SetArgs(args);

		if (command.GetArgs().empty())
		{
			return PointSegment(command);
		}

		return ProcessCommand(command);
	}

	/**
	* Warning, this should only be used when modifying live gcode, such as when
	* expanding an arc or canned cycle into line segments.
	*/
	void GCodeParser::SetLastGcodeCommand(float num)
	{
		mLastGcodeCommand = num;
	}

	/**
	* Gets the point at the end of the list.
	*/
	vec3 GCodeParser::GetCurrentPoint()
	{
		return mCurrentPoint;
	}

	/**
	* Expands the last point in the list if it is an arc according to the
	* the Parsers Settings.
	*/
	vector<PointSegment> GCodeParser::ExpandArc(const GCodeCommand& parent)
	{
		info("GcodeParser::expandArc()");
		PointSegment& startSegment = mPoints[mPoints.size() - 2];
		PointSegment& lastSegment = mPoints[mPoints.size() - 1];

		vector<PointSegment> empty;

		// Can only expand arcs.
		if (!lastSegment.IsArc())
		{
			return empty;
		}

		// Get precalculated stuff.
		vec3& start = startSegment.GetPoint();
		vec3& end = lastSegment.GetPoint();
		vec3 center = lastSegment.Center();
		double radius = lastSegment.GetRadius();
		bool clockwise = lastSegment.IsClockwise();
		PointSegment::planes plane = startSegment.Plane();

		//
		// Start expansion.
		//

		vector<vec3> expandedPoints = GeneratePointsAlongArcBDring(
			plane, start, end, center, clockwise, radius,
			mSmallArcThreshold, mSmallArcSegmentLength, false
		);

		// Validate output of expansion.
		if (expandedPoints.size() == 0)
		{
			return empty;
		}

		// Remove the last point now that we're about to expand it.
        mPoints.pop_back();
		mCommandNumber--;

		// Initialize return value
		vector<PointSegment> psl;

		// Create line segments from points.

		vector<vec3>::iterator psi = expandedPoints.begin();

		// skip first element.
		if (psi != expandedPoints.end()) psi++;


		while (psi != expandedPoints.end())
		{
			PointSegment temp = PointSegment(parent, *psi, mCommandNumber++);
            psi++;
			temp.SetIsArc(true);
			temp.SetIsMetric(lastSegment.IsMetric());
			mPoints.push_back(temp);
			psl.push_back(temp);
		}

		// Update the new endpoint.
		mCurrentPoint = vec3(mPoints.back().GetPoint());

		return psl;
	}

	vector<PointSegment>& GCodeParser::GetPointSegmentList()
	{
		return mPoints;
	}

	double GCodeParser::GetTraverseSpeed() const
	{
		return mTraverseSpeed;
	}

	void GCodeParser::SetTraverseSpeed(double traverseSpeed)
	{
		mTraverseSpeed = traverseSpeed;
	}

	int GCodeParser::GetCommandNumber() const
	{
		return mCommandNumber - 1;
	}

	PointSegment GCodeParser::ProcessCommand(const GCodeCommand& command)
	{
		vector<float> gCodes;
		PointSegment ps = PointSegment(command);

		vector<string> args = command.GetArgs();

		// Handle F code
		double speed = ParseCoord(args, 'F');
		if (!isnan(speed))
		{
			mLastSpeed = mIsMetric ? speed : speed * 25.4;
		}

		// Handle S code
		double spindleSpeed = ParseCoord(args, 'S');
		if (!isnan(spindleSpeed))
		{
			mLastSpindleSpeed = spindleSpeed;
		}

		// Handle P code
		double dwell = ParseCoord(args, 'P');
		if (!isnan(dwell))
		{
            mPoints.back().SetDwell(dwell);
		}

		// handle G codes.
		gCodes = ParseCodes(args, 'G');

		// If there was no command, add the implicit one to the party.
		if (gCodes.empty() && mLastGcodeCommand != -1.0f)
		{
			gCodes.push_back(mLastGcodeCommand);
		}

		for (float code : gCodes)
		{
			ps = HandleGCode(command, code);
		}

		return ps;
	}

	PointSegment
    GCodeParser::AddLinearPointSegment
    (const GCodeCommand& parent, const vec3 &nextPoint, bool fastTraverse)
	{
		PointSegment ps = PointSegment(parent, nextPoint, mCommandNumber++);

		bool zOnly = false;

		// Check for z-only
		if (mCurrentPoint.x == nextPoint.x && mCurrentPoint.y == nextPoint.y &&
			mCurrentPoint.z != nextPoint.z)
		{
			zOnly = true;
		}

		ps.SetIsMetric(mIsMetric);
		ps.SetIsZMovement(zOnly);
		ps.SetIsRapidMovement(fastTraverse);
		ps.SetIsAbsolute(mInAbsoluteMode);
		ps.SetSpeed(fastTraverse ? mTraverseSpeed : mLastSpeed);
		ps.SetSpindleSpeed(mLastSpindleSpeed);
		mPoints.push_back(ps);

		// Save off the endpoint.
		mCurrentPoint = nextPoint;
		return ps;
	}

	PointSegment
    GCodeParser::AddArcPointSegment
    (const GCodeCommand& cmd, const vec3 &nextPoint, bool clockwise, const vector<string> &args)
	{
		PointSegment ps = PointSegment(cmd, nextPoint, mCommandNumber++);

		vec3 center = UpdateCenterWithCommand(args, mCurrentPoint, nextPoint, mInAbsoluteIJKMode, clockwise);
		double radius = ParseCoord(args, 'R');

		// Calculate radius if necessary.
		if (isnan(radius))
		{
			mat4 m(1.0f);

			switch (mCurrentPlane)
			{
				case PointSegment::XY:
					break;
				case PointSegment::ZX:
					m = rotate(m, 90.0f, vec3(1.0f, 0.0f, 0.0f));
					break;
				case PointSegment::YZ:
					m = rotate(m, -90.0f, vec3(0.0f, 1.0f, 0.0f));
					break;
			}

			radius = sqrt
			(
				pow(static_cast<double>(((m * vec4(mCurrentPoint,1.0f)).x - (m * vec4(center,1.0f)).x)), 2.0) +
				pow(static_cast<double>(((m * vec4(mCurrentPoint,1.0f)).y - (m * vec4(center,1.0f)).y)), 2.0)
			);
		}

		ps.SetIsMetric(mIsMetric);
		ps.SetArcCenter(center);
		ps.SetIsArc(true);
		ps.SetRadius(radius);
		ps.SetIsClockwise(clockwise);
		ps.SetIsAbsolute(mInAbsoluteMode);
		ps.SetSpeed(mLastSpeed);
		ps.SetSpindleSpeed(mLastSpindleSpeed);
		ps.SetPlane(mCurrentPlane);
		mPoints.push_back(ps);

		// Save off the endpoint.
		mCurrentPoint = nextPoint;
		return ps;
	}

	void GCodeParser::HandleMCode(float code, const vector<string> &args)
	{
		double spindleSpeed = ParseCoord(args, 'S');
		if (!isnan(spindleSpeed))
		{
			mLastSpindleSpeed = spindleSpeed;
		}
	}

	PointSegment
    GCodeParser::HandleGCode(const GCodeCommand& command, float code)
	{
		info("GcodeParser: handleGCode {}", code );//<< args;
		vector<string> args = command.GetArgs();

		PointSegment ps(command);
		vec3 nextPoint = UpdatePointWithCommand(args, mCurrentPoint, mInAbsoluteMode);

		if (code == GCODE_RAPID)
		{
			ps = AddLinearPointSegment(command, nextPoint, true);
		}
		else if (code == GCODE_LINEAR_INTERPOLATION)
		{
			ps = AddLinearPointSegment(command, nextPoint, false);
		}
		else if (code == GCODE_STRAIGHT_PROBE)
		{
			ps = AddLinearPointSegment(command, nextPoint, false);
		}
		else if (code == GCODE_ARC_MOVE_IJK)
		{
			ps = AddArcPointSegment(command, nextPoint, true, args);
		}
		else if (code == GCODE_ARC_MOVE_RP)
		{
			ps = AddArcPointSegment(command, nextPoint, false, args);
		}
		else if (code == GCODE_PLANE_XY)
		{
			mCurrentPlane = PointSegment::XY;
		}
		else if (code == GCODE_PLANE_ZX)
		{
			mCurrentPlane = PointSegment::ZX;
		}
		else if (code == GCODE_PLANE_YZ)
		{
			mCurrentPlane = PointSegment::YZ;
		}
		else if (code == GCODE_UNITS_INCHES)
		{
			mIsMetric = false;
		}
		else if (code == GCODE_UNITS_MM)
		{
			mIsMetric = true;
		}
		else if (code == GCODE_DISTANCE_ABSOLUTE)
		{
			mInAbsoluteMode = true;
		}
		else if (code == GCODE_DISTANCE_ABSOLUTE_IJK)
		{
			mInAbsoluteIJKMode = true;
		}
		else if (code == GCODE_DISTANCE_INCREMENTAL)
		{
			mInAbsoluteMode = false;
		}
		else if (code == GCODE_DISTANCE_INCREMENTAL_IJK)
		{
			mInAbsoluteIJKMode = false;
		}

		if  (
			 code == GCODE_RAPID ||
			 code == GCODE_LINEAR_INTERPOLATION ||
			 code == GCODE_ARC_MOVE_IJK ||
			 code == GCODE_ARC_MOVE_RP ||
			 code == GCODE_STRAIGHT_PROBE
			)
		{
			mLastGcodeCommand = code;
		}
		return ps;
	}

	/*
	vector<string> GcodeParser::preprocessCommands(vector<string> commands)
	{

		vector<string> result;

		foreach (string command, commands)
		{
			result.push_back(preprocessCommand(command));
		}

		return result;
	}
	*/

	/*
	vector<string> GcodeParser::preprocessCommand(string command)
	{

		vector<string> result;
		bool hasComment = false;

		// Remove comments from command.
		string newCommand = removeComment(command);
		string rawCommand = newCommand;
		hasComment = (newCommand.length() != command.length());

		if (mRemoveAllWhitespace)
		{
			newCommand = removeAllWhitespace(newCommand);
		}

		if (newCommand.length() > 0)
		{

			// Override feed speed
			if (mSpeedOverride > 0)
			{
				newCommand = overrideSpeed(newCommand, mSpeedOverride);
			}

			if (mTruncateDecimalLength > 0)
			{
				newCommand = truncateDecimals(mTruncateDecimalLength, newCommand);
			}

			// If this is enabled we need to Parse the gcode as we go along.
			if (mConvertArcsToLines)
			{ // || this.expandCannedCycles) {
				vector<string> arcLines = convertArcsToLines(newCommand);
				if (arcLines.length() > 0)
				{
					result.push_back(arcLines);
				}
				else
				{
					result.push_back(newCommand);
				}
			}
			else if (hasComment)
			{
				// Maintain line level comment.
				result.push_back(command.replace(rawCommand, newCommand));
			}
			else
			{
				result.push_back(newCommand);
			}
		}
		else if (hasComment)
		{
			// Reinsert comment-only lines.
			result.push_back(command);
		}

		return result;
	}
	*/

	/*
	vector<string> GcodeParser::convertArcsToLines(string command)
	{

		vector<string> result;

		vec3 *start = &mCurrentPoint;

		PointSegment* ps = addCommand(command);

		if (!ps->isArc())
		{
			return result;
		}

		vector<PointSegment*> psl = expandArc();

		if (psl.length() == 0)
		{
			return result;
		}

		// Create an array of new commands out of the of the segments in psl.
		// Don't add them to the gcode Parser since it is who expanded them.
		foreach (PointSegment* segment, psl)
		{
			//Point3d end = segment.point();
			vec3 *end = segment->GetPointHandle();
			result.push_back(generateG1FromPoints(*start, *end, mInAbsoluteMode, mTruncateDecimalLength));
			start = segment->GetPointHandle();
		}
		return result;
	}
	*/

	/**
	* Searches the command string for an 'f' and replaces the speed value
	* between the 'f' and the next space with a percentage of that speed.
	* In that way all speed values become a ratio of the provided speed
	* and don't Get overridden with just a fixed speed.
	*/
	string GCodeParser::OverrideSpeed(const string& command, double speed)
	{
		static regex re("[Ff]([0-9.]+)");
		static smatch m;
        string tmp = command;
		if (regex_search(tmp,m,re))
		{
            if (m.size() == 2)
            {
				double speedMagnitude = (speed/100.0); // from percentage to magnitude
				ssub_match whole_str = m[0];
                auto first = whole_str.first;
				auto last = whole_str.second;
                auto speed_str = m[1];
            	tmp.replace(first,last, "F" + std::to_string(stod(speed_str) * speedMagnitude));
            }
		}
		return tmp;
	}

	/**
	* Searches the command string for an 'f' and replaces the speed value
	* between the 'f' and the next space with a percentage of that speed.
	* In that way all speed values become a ratio of the provided speed
	* and don't Get overridden with just a fixed speed.
	*/
	GCodeCommand GCodeParser::OverrideSpeed(const GCodeCommand& command, double speed)
	{
		string cmd = command.GetCommand();
        static smatch m;
		static regex re("[Ff]([0-9.]+)");
		if (regex_search(cmd,m,re))
		{
            if (m.size() == 2)
            {
				double speedMagnitude = (speed/100.0); // from percentage to magnitude
				ssub_match whole_str = m[0];
                auto first = whole_str.first;
				auto last = whole_str.second;
                auto speed_str = m[1];
				cmd.replace(first,last, "F" + std::to_string(stod(speed_str) * speedMagnitude));
            }
		}
		// TODO - Leak Spin
		GCodeCommand* overriddenFR = new GCodeCommand(command);
		overriddenFR->SetCommand(cmd);
		return overriddenFR;
	}

	/**
	* Removes any comments within parentheses or beginning with a semi-colon.
	*/
	string GCodeParser::RemoveComment(const string& command)
	{
		static regex rx1("\\(+[^\\(]*\\)+");
		static regex rx2(";.*");

        string tmp = command;

		// Remove any comments within ( parentheses ) using regex "\([^\(]*\)"
        if (command.find('(') != string::npos)
        {
            smatch m;
            if (regex_search(tmp,m,rx1))
            {
            	tmp.erase(m[0].first,m[0].second);
            }
        }

		// Remove any comment beginning with ';' using regex ";.*"

        {
            smatch m;
            if (regex_search(tmp,m,rx2))
            {
            	tmp.erase(m[0].first,m[0].second);
            }
        }

		return Util::trim_copy(tmp);
	}

	/**
	* Searches for a comment in the input string and returns the first match.
	*/
	string GCodeParser::ParseComment(const string& command)
	{
		// REGEX: Find any comment, includes the comment characters:
		// "(?<=\()[^\(\)]*|(?<=\;)[^;]*"
		// "(?<=\\()[^\\(\\)]*|(?<=\\;)[^;]*"

		debug("GcodeParser: Parsing comment in command {}",  command);
		static regex re("(\\([^\\(\\)]*\\)|;[^;].*)");
        static smatch m;

		if (regex_search(command,m,re))
		{
			string cap = m[0];
			debug("GcodeParser: Found comment right here {}", cap);
			return cap;
		}
		return "";
	}

	string GCodeParser::TruncateDecimals(int length, const string& command)
	{
		static regex re("(\\d*\\.\\d*)");
        static smatch m;

        string tmp = command;

        while (regex_search(tmp,m,re))
        {
            string num = m[0];
            stringstream s;
            s << fixed << setprecision(length) << stod(num);
			string newNum = string(s.str());
            tmp.replace(m[0].first,m[0].second,newNum);
        }

		return tmp;
	}

	string GCodeParser::RemoveAllWhitespace(const string& command)
	{
		static regex rx("\\s");
        smatch m;
        string tmp = command;

        while (regex_search(tmp,m,rx))
        {
           tmp.erase(m[0].first, m[0].second);
        }

		return tmp;
	}

	vector<float> GCodeParser::ParseCodes(const vector<string>& args, char code)
	{
		vector<float> l;

		for (string s : args)
		{
			if (s.length() > 0 && std::toupper(s[0]) == code)
			{
				l.push_back(stod(s.substr(1)));
			}
		}

		return l;
	}

	vector<int> GCodeParser::ParseGCodes(const string& command)
	{
		static regex re("[Gg]0*(\\d+)");
        smatch m;

		vector<int> codes;
		int pos = 0;

		if (regex_search(command, m, re))
		{
            for (int i=1; i<m.size(); i++)
            {
				codes.push_back(stoi(m[i].str()));
            }
		}

		return codes;
	}

	vector<int> GCodeParser::ParseMCodes(const string& command)
	{
		static regex re("[Mm]0*(\\d+)");
        smatch sm;

		vector<int> codes;
		int pos = 0;

        if (regex_search(command,sm,re))
        {
           for (int i=0; i<sm.size(); i++)
           {
               codes.push_back(stoi(sm[i]));
           }
        }

		return codes;
	}

	/**
	* Update a point given the arguments of a command.
	*/
	vec3 GCodeParser::UpdatePointWithCommand
    (const string &command, const vec3 &initial, bool absoluteMode)
	{
		vector<string> l = SplitCommand(command);
		return UpdatePointWithCommand(l, initial, absoluteMode);
	}

	/**
	* Update a point given the arguments of a command, using a pre-Parsed list.
	*/
	vec3
    GCodeParser::UpdatePointWithCommand
    (const vector<string> &commandArgs, const vec3 &initial, bool absoluteMode)
	{
		double x = NAN;
		double y = NAN;
		double z = NAN;
		char c;

		for (int i = 0; i < commandArgs.size(); i++)
		{
			if (commandArgs.at(i).length() > 0)
			{
				c = std::toupper(commandArgs.at(i).at(0));
				switch (c)
				{
				case 'X':
					try
                    {
						x = stod(commandArgs.at(i).substr(1));
                    }
                    catch (exception& e)
                    {
                    	info("No X Value found in command");
                    }
					break;
				case 'Y':
                    try
                    {
						y = stod(commandArgs.at(i).substr(1));
                    }
                    catch (exception& e)
                    {
                    	info("No Y Value found in command");
                    }

					break;
				case 'Z':
					try
					{
						z = stod(commandArgs.at(i).substr(1));
                    }
                    catch (exception& e)
                    {
                    	info("No Z Value found in command");
                    }
					break;
				}
			}
		}

		return UpdatePointWithCommand(initial, x, y, z, absoluteMode);
	}

	/**
	* Update a point given the new coordinates.
	*/
	vec3
    GCodeParser::UpdatePointWithCommand
    (const vec3 &initial, double x, double y, double z, bool absoluteMode)
	{
		vec3 newPoint(initial);// = vec3(qQNaN(),qQNaN(),qQNaN());

		if (absoluteMode)
		{
			if (!isnan(x)) newPoint.x = x;
			if (!isnan(y)) newPoint.y = y;
			if (!isnan(z)) newPoint.z = z;
		}
		else
		{
			if (!isnan(x)) newPoint.x = initial.x + x;
			if (!isnan(y)) newPoint.y = initial.y + y;
			if (!isnan(z)) newPoint.z = initial.z + z;
		}

		return newPoint;
	}

	vec3
    GCodeParser::UpdateCenterWithCommand
    (const vector<string>& commandArgs, const vec3& initial, const vec3& nextPoint,
     bool absoluteIJKMode, bool clockwise)
	{
		double i = NAN;
		double j = NAN;
		double k = NAN;
		double r = NAN;
		char c;

		for (string t : commandArgs)
		{
			if (t.length() > 0)
			{
				c = std::toupper(t[0]);
				switch (c)
				{
				case 'I':
					i = stod(t.substr(1));
					break;
				case 'J':
					j = stod(t.substr(1));
					break;
				case 'K':
					k = stod(t.substr(1));
					break;
				case 'R':
					r = stod(t.substr(1));
					break;
				}
			}
		}

		if (isnan(i) && isnan(j) && isnan(k))
		{
			return ConvertRToCenter(initial, nextPoint, r, absoluteIJKMode, clockwise);
		}

		return UpdatePointWithCommand(initial, i, j, k, absoluteIJKMode);
	}

	string GCodeParser::GenerateG1FromPoints(const vec3& start, const vec3& end,
                                             bool absoluteMode, int precision)
	{
		stringstream sb;
		sb << "G1";

		if (absoluteMode)
		{
            if (!isnan(end.x))
            {
        		sb << "X" << fixed << setprecision(precision) << end.x;
                //sb.push_back(s.str());//"X" + std::to_string(end.x, 'f', precision));
            }

            if (!isnan(end.y))
            {
        		sb << "Y" << fixed << setprecision(precision) << end.x;
                //sb.push_back(s.str());//"Y" + std::to_string(end.y, 'f', precision));
            }

            if (!isnan(end.z))
            {
        		sb << "Z" << fixed << setprecision(precision) << end.x;
                //sb.push_back("Z" + std::to_string(end.z, 'f', precision));
            }
		}
		else
		{
            if (!isnan(end.x))
            {
                sb << "X" << fixed << setprecision(precision) << end.x - start.x;
            }
            if (!isnan(end.y))
            {
                sb << "Y" << fixed << setprecision(precision) << end.y - start.y;
            }
            if (!isnan(end.z))
            {
                sb << "Z" << fixed << setprecision(precision) << end.z - start.z;
            }
		}

		return sb.str();
	}

	/**
	* Splits a gcode command by each word/argument, doesn't care about spaces.
	* This command is about the same speed as the string.split(" ") command,
	* but might be a little faster using precompiled regex.
	*/
	vector<string> GCodeParser::SplitCommand(const string& command)
	{
		vector<string> l;
		bool readNumeric = false;
		string sb;

		const char* ba = command.c_str();
		const char* cmd = ba; // Direct access to string data
		char c;

		for (int i = 0; i < command.length(); i++)
		{
			c = cmd[i];

			if (readNumeric && !IsDigit(c) && c != '.')
			{
				readNumeric = false;
				l.push_back(sb);
				sb.clear();
				if (IsLetter(c)) sb.push_back(c);
			}
			else if (IsDigit(c) || c == '.' || c == '-')
			{
				sb.push_back(c);
				readNumeric = true;
			}
			else if (IsLetter(c))
			{
				sb.push_back(c);
			}
		}

		if (sb.length() > 0)
		{
			l.push_back(sb);
		}

	//    QChar c;

	//    for (int i = 0; i < command.length(); i++) {
	//        c = command[i];

	//        if (readNumeric && !c.isDigit() && c != '.') {
	//            readNumeric = false;
	//            l.push_back(sb);
	//            sb = "";
	//            if (c.isLetter()) sb.push_back(c);
	//        } else if (c.isDigit() || c == '.' || c == '-') {
	//            sb.push_back(c);
	//            readNumeric = true;
	//        } else if (c.isLetter()) sb.push_back(c);
	//    }

	//    if (sb.length() > 0) l.push_back(sb);

		return l;
	}

	// TODO: Replace everything that uses this with a loop that loops through
	// the string and creates a hash with all the values.
	double GCodeParser::ParseCoord(const vector<string>& argList, char c)
	{
	//    int n = argList.length();

	//    for (int i = 0; i < n; i++) {
	//        if (argList[i].length() > 0 && argList[i][0].toUpper() == c) return argList[i].mid(1).toDouble();
	//    }

		for(string t : argList)
		{
			if (t.length() > 0 && std::toupper(t[0]) == c) return stod(t.substr(1));
		}
		return NAN;
	}

	//static public List<String> convertArcsToLines(Point3d start, Point3d end) {
	//    List<String> l = new ArrayList<String>();

	//    return l;
	//}

	vec3 GCodeParser::ConvertRToCenter
    (const vec3& start, const vec3& end, double rad, bool absoluteIJK, bool clockwise)
	{
		double radius = rad;
		vec3 center;

		double x = end.x - start.x;
		double y = end.y - start.y;

		double height_x2_div_diameter = 4 * radius * radius - x * x - y * y;
		if (height_x2_div_diameter < 0)
		{
			//cout << "Error computing arc radius.";
		}
		height_x2_div_diameter = (-sqrt(height_x2_div_diameter)) / hypot(x, y);

		if (!clockwise)
		{
			height_x2_div_diameter = -height_x2_div_diameter;
		}

		// Special message from gcoder to software for which radius
		// should be used.
		if (radius < 0)
		{
			height_x2_div_diameter = -height_x2_div_diameter;
			// TODO: Places that use this need to run ABS on radius.
			radius = -radius;
		}

		double offSetX = 0.5 * (x - (y * height_x2_div_diameter));
		double offSetY = 0.5 * (y + (x * height_x2_div_diameter));

		if (!absoluteIJK)
		{
			center.x = start.x + offSetX;
			center.y = start.y + offSetY;
		}
		else
		{
			center.x = offSetX;
			center.y = offSetY;
		}

		return center;
	}

	/**
	* Return the angle in radians when going from start to end.
	*/
	double GCodeParser::GetAngle(const vec3& start, const vec3& end)
	{
		double deltaX = end.x - start.x;
		double deltaY = end.y - start.y;

		double angle = 0.0;

		if (deltaX != 0)
		{ // prevent div by 0
			// it helps to know what quadrant you are in
			if (deltaX > 0 && deltaY >= 0)
			{ // 0 - 90
				angle = atan(deltaY / deltaX);
			}
			else if (deltaX < 0 && deltaY >= 0)
			{ // 90 to 180
				angle = M_PI - fabs(atan(deltaY / deltaX));
			}
			else if (deltaX < 0 && deltaY < 0)
			{ // 180 - 270
				angle = M_PI + fabs(atan(deltaY / deltaX));
			}
			else if (deltaX > 0 && deltaY < 0)
			{ // 270 - 360
				angle = M_PI * 2 - fabs(atan(deltaY / deltaX));
			}
		}
		else
		{
			// 90 deg
			if (deltaY > 0)
			{
				angle = M_PI / 2.0;
			}
			// 270 deg
			else
			{
				angle = M_PI * 3.0 / 2.0;
			}
		}

		return angle;
	}

	double GCodeParser::CalculateSweep(double startAngle, double endAngle, bool isCw)
	{
		double sweep;

		// Full circle
		if (startAngle == endAngle)
		{
			sweep = (M_PI * 2);
			// Arcs
		}
		else
		{
			// Account for full circles and end angles of 0/360
			if (endAngle == 0)
			{
				endAngle = M_PI * 2;
			}
			// Calculate distance along arc.
			if (!isCw && endAngle < startAngle)
			{
				sweep = ((M_PI * 2 - startAngle) + endAngle);
			}
			else if (isCw && endAngle > startAngle)
			{
				sweep = ((M_PI * 2 - endAngle) + startAngle);
			}
			else
			{
				sweep = fabs(endAngle - startAngle);
			}
		}

		return sweep;
	}

	/**
	* Generates the points along an arc including the start and end points.
	*
	* ArcPrecision = Minimum length of line in arc
	*/
	vector<vec3>
	GCodeParser::GeneratePointsAlongArcBDring
	(
			PointSegment::planes plane, vec3& start, vec3& end,
			vec3& center, bool clockwise, double rad, double minArcLength,
			double arcPrecision, bool arcDegreeMode
	){
		double radius = rad;
		// Rotate vectors according to plane
		mat4 m(1.0f);

		switch (plane)
		{
		case PointSegment::XY:
			break;
		case PointSegment::ZX:
			m = rotate(m, 90.0f, vec3(1.0, 0.0, 0.0));
			break;
		case PointSegment::YZ:
			m = rotate(m, -90.0f, vec3(0.0, 1.0, 0.0));
			break;
		}
		start = m * vec4(start,1.0f);
		end = m * vec4(end,1.0f);
		center = m * vec4(center,1.0f);

		// Check center
		if (isnan(center.length()))
		{
			return vector<vec3>();
		}

		// Calculate radius if necessary.
		if (radius == 0)
		{
			radius = sqrt(pow((double)(start.x - center.x), 2.0) + pow((double)(end.y - center.y), 2.0));
		}

		double startAngle = GetAngle(center, start);
		double endAngle = GetAngle(center, end);
		double sweep = CalculateSweep(startAngle, endAngle, clockwise);

		// Convert units.
		double arcLength = sweep * radius;

		// If this arc doesn't meet the minimum threshold, don't expand.
	//    if (minArcLength > 0 && arcLength < minArcLength) {
	//        vector<vec3> empty;
	//        return empty;
	//    }

		int numPoints;

		if (arcDegreeMode && arcPrecision > 0)
		{
			numPoints = max(1.0, sweep / (M_PI * arcPrecision / 180));
		}
		else
		{
			if (arcPrecision <= 0 && minArcLength > 0)
			{
				arcPrecision = minArcLength;
			}
			numPoints = (int)ceil(arcLength/arcPrecision);
		}

		return GeneratePointsAlongArcBDring(
			plane, start, end, center, clockwise,
			radius, startAngle, sweep, numPoints
		);
	}

	/**
	* Generates the points along an arc including the start and end points.
	*/
	vector<vec3>
	GCodeParser::GeneratePointsAlongArcBDring
	(
		PointSegment::planes plane, vec3& p1, vec3& p2,
		vec3& center, bool isCw, double radius, double startAngle,
		double sweep, int numPoints
	){
		// Prepare rotation matrix to restore plane
		mat4 m(1.0f);
		switch (plane)
		{
		case PointSegment::XY:
			break;
		case PointSegment::ZX:
			m = rotate(m, -90.0f, vec3(1.0, 0.0, 0.0));
			break;
		case PointSegment::YZ:
			m = rotate(m, 90.0f, vec3(0.0, 1.0, 0.0));
			break;
		}

		vec3 lineEnd(p2.x, p2.y, p1.z);
		vector<vec3> segments;
		double angle;

		// Calculate radius if necessary.
		if (radius == 0)
		{
			radius = sqrt(pow((double)(p1.x - center.x), 2.0) + pow((double)(p1.y - center.y), 2.0));
		}

		double zIncrement = (p2.z - p1.z) / numPoints;
		for (int i = 1; i < numPoints; i++)
		{
			if (isCw)
			{
				angle = (startAngle - i * sweep / numPoints);
			}
			else
			{
				angle = (startAngle + i * sweep / numPoints);
			}

			if (angle >= M_PI * 2)
			{
				angle = angle - M_PI * 2;
			}

			lineEnd.x = (cos(angle) * radius + center.x);
			lineEnd.y = (sin(angle) * radius + center.y);
			lineEnd.z = (lineEnd.z + zIncrement);

			segments.push_back(m * vec4(lineEnd,1.0));
		}

		segments.push_back(m * vec4(p2,1.0));

		return segments;
	}

	bool GCodeParser::IsDigit(char c)
	{
		return c > 47 && c < 58;
	}

	bool GCodeParser::IsLetter(char c)
	{
		return (c > 64 && c < 91) || (c > 96 && c < 123);
	}

	char GCodeParser::ToUpper(char c)
	{
		return (c > 96 && c < 123) ? c - 32 : c;
	}
}
