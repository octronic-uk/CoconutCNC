// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeParser.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "GcodeParser.h"

#include <iostream>
#include <cmath>
#include <glm/mat4x4.hpp>

using std::cout;
using std::endl;
using std::isnan;

namespace Coconut
{
	GcodeParser::GcodeParser()
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
		  mLastSpindleSpeed(0)
	{
		cout << "GcodeParser: Constructing";
		reset(vec3(NAN));
	}

	GcodeParser::~GcodeParser()
	{
		cout << "GcodeParser: Destructing";
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

	bool GcodeParser::getConvertArcsToLines()
	{
		return mConvertArcsToLines;
	}

	void GcodeParser::setConvertArcsToLines(bool convertArcsToLines)
	{
		mConvertArcsToLines = convertArcsToLines;
	}

	bool GcodeParser::getRemoveAllWhitespace()
	{
		return mRemoveAllWhitespace;
	}

	void GcodeParser::setRemoveAllWhitespace(bool removeAllWhitespace)
	{
		mRemoveAllWhitespace = removeAllWhitespace;
	}

	double GcodeParser::getSmallArcSegmentLength()
	{
		return mSmallArcSegmentLength;
	}

	void GcodeParser::setSmallArcSegmentLength(double smallArcSegmentLength)
	{
		mSmallArcSegmentLength = smallArcSegmentLength;
	}

	double GcodeParser::getSmallArcThreshold()
	{
		return mSmallArcThreshold;
	}

	void GcodeParser::setSmallArcThreshold(double smallArcThreshold)
	{
		mSmallArcThreshold = smallArcThreshold;
	}

	double GcodeParser::getSpeedOverride()
	{
		return mSpeedOverride;
	}

	void GcodeParser::setSpeedOverride(double speedOverride)
	{
		mSpeedOverride = speedOverride;
	}

	int GcodeParser::getTruncateDecimalLength()
	{
		return mTruncateDecimalLength;
	}

	void GcodeParser::setTruncateDecimalLength(int truncateDecimalLength)
	{
		mTruncateDecimalLength = truncateDecimalLength;
	}

	// Resets the current state.
	void GcodeParser::reset(const vec3& initialPoint)
	{

		cout << "GcodeParser: Reseting parser with initial point"
             << initialPoint.x << ","
             << initialPoint.y << ","
             << initialPoint.z << endl;
		mPoints.clear();
		// The unspoken home location.
		mCurrentPoint = initialPoint;
		mCurrentPlane = PointSegment::XY;
		mPoints.push_back(new PointSegment(nullptr, mCurrentPoint, -1));
	}

	/**
	* Add a command to be processed.
	*/
	PointSegment* GcodeParser::addCommand(GCodeCommand* command)
	{
		string stripped = removeComment(command->getCommand());
		vector<string> args = splitCommand(stripped);
		command->setArgs(args);

		if (command->getArgs().empty())
		{
			return new PointSegment(command);
		}

		return processCommand(command);
	}

	/**
	* Warning, this should only be used when modifying live gcode, such as when
	* expanding an arc or canned cycle into line segments.
	*/
	void GcodeParser::setLastGcodeCommand(float num)
	{
		mLastGcodeCommand = num;
	}

	/**
	* Gets the point at the end of the list.
	*/
	vec3 GcodeParser::getCurrentPoint()
	{
		return mCurrentPoint;
	}

	/**
	* Expands the last point in the list if it is an arc according to the
	* the parsers settings.
	*/
	vector<PointSegment*> GcodeParser::expandArc(GCodeCommand* parent)
	{
		//cout << "GcodeParser::expandArc()";
		PointSegment* startSegment = mPoints[mPoints.size() - 2].data();
		PointSegment* lastSegment = mPoints[mPoints.size() - 1].data();

		vector<PointSegment*> empty;

		// Can only expand arcs.
		if (!lastSegment->isArc())
		{
			return empty;
		}

		// Get precalculated stuff.
		vec3* start = startSegment->getPointHandle();
		vec3* end = lastSegment->getPointHandle();
		vec3 center = lastSegment->center();
		double radius = lastSegment->getRadius();
		bool clockwise = lastSegment->isClockwise();
		PointSegment::planes plane = startSegment->plane();

		//
		// Start expansion.
		//

		vector<vec3> expandedPoints =
			generatePointsAlongArcBDring
			(
				plane, *start, *end, center, clockwise, radius,
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
		vector<PointSegment*> psl;

		// Create line segments from points.

		vector<vec3>::iterator psi = expandedPoints.begin();

		// skip first element.
		if (psi != expandedPoints.end()) psi++;


		while (psi != expandedPoints.end())
		{
			auto temp = new PointSegment(parent, *psi++, mCommandNumber++);
			temp->setIsArc(true);
			temp->setIsMetric(lastSegment->isMetric());
			mPoints.push_back(temp);
			psl.push_back(temp);
		}

		// Update the new endpoint.
		mCurrentPoint = vec3(*mPoints.last()->getPointHandle());

		return psl;
	}

	vector<PointSegment*> GcodeParser::getPointSegmentHandlesList()
	{
		vector<PointSegment*> handles;
		for (auto ps : mPoints)
            handles.push_back(ps);
		return handles;
	}

	double GcodeParser::getTraverseSpeed() const
	{
		return mTraverseSpeed;
	}

	void GcodeParser::setTraverseSpeed(double traverseSpeed)
	{
		mTraverseSpeed = traverseSpeed;
	}

	int GcodeParser::getCommandNumber() const
	{
		return mCommandNumber - 1;
	}

	PointSegment* GcodeParser::processCommand(GCodeCommand* command)
	{
		vector<float> gCodes;
		PointSegment* ps = new PointSegment(command);

		vector<string> args = command->getArgs();

		// Handle F code
		double speed = parseCoord(args, 'F');
		if (!isnan(speed))
		{
			mLastSpeed = mIsMetric ? speed : speed * 25.4;
		}

		// Handle S code
		double spindleSpeed = parseCoord(args, 'S');
		if (!isnan(spindleSpeed))
		{
			mLastSpindleSpeed = spindleSpeed;
		}

		// Handle P code
		double dwell = parseCoord(args, 'P');
		if (!isnan(dwell))
		{
            mPoints.back()->setDwell(dwell);
		}

		// handle G codes.
		gCodes = parseCodes(args, 'G');

		// If there was no command, add the implicit one to the party.
		if (gCodes.empty() && mLastGcodeCommand != -1.0f)
		{
			gCodes.push_back(mLastGcodeCommand);
		}

		for (float code : gCodes)
		{
			ps = handleGCode(command, code);
		}

		return ps;
	}

	PointSegment* GcodeParser::addLinearPointSegment(GCodeCommand* parent, const vec3 &nextPoint, bool fastTraverse)
	{
		auto ps = new PointSegment(parent, nextPoint, mCommandNumber++);

		bool zOnly = false;

		// Check for z-only
		if (mCurrentPoint.x == nextPoint.x && mCurrentPoint.y == nextPoint.y &&
			mCurrentPoint.z != nextPoint.z)
		{
			zOnly = true;
		}

		ps->setIsMetric(mIsMetric);
		ps->setIsZMovement(zOnly);
		ps->setIsRapidMovement(fastTraverse);
		ps->setIsAbsolute(mInAbsoluteMode);
		ps->setSpeed(fastTraverse ? mTraverseSpeed : mLastSpeed);
		ps->setSpindleSpeed(mLastSpindleSpeed);
		mPoints.push_back(ps);

		// Save off the endpoint.
		mCurrentPoint = nextPoint;
		return ps;
	}

	PointSegment* GcodeParser::addArcPointSegment(GCodeCommand* cmd, const vec3 &nextPoint, bool clockwise, const vector<string> &args)
	{
		auto ps = new PointSegment(cmd, nextPoint, mCommandNumber++);

		vec3 center = updateCenterWithCommand(args, mCurrentPoint, nextPoint, mInAbsoluteIJKMode, clockwise);
		double radius = parseCoord(args, 'R');

		// Calculate radius if necessary.
		if (isnan(radius))
		{
			mat4 m;
			m.setToIdentity();
			switch (mCurrentPlane)
			{
				case PointSegment::XY:
					break;
				case PointSegment::ZX:
					m.rotate(90, 1.0, 0.0, 0.0);
					break;
				case PointSegment::YZ:
					m.rotate(-90, 0.0, 1.0, 0.0);
					break;
			}

			radius = sqrt
			(
				pow(static_cast<double>(((m * mCurrentPoint).x() - (m * center).x())), 2.0) +
				pow(static_cast<double>(((m * mCurrentPoint).y() - (m * center).y())), 2.0)
			);
		}

		ps->setIsMetric(mIsMetric);
		ps->setArcCenter(center);
		ps->setIsArc(true);
		ps->setRadius(radius);
		ps->setIsClockwise(clockwise);
		ps->setIsAbsolute(mInAbsoluteMode);
		ps->setSpeed(mLastSpeed);
		ps->setSpindleSpeed(mLastSpindleSpeed);
		ps->setPlane(mCurrentPlane);
		mPoints.push_back(ps);

		// Save off the endpoint.
		mCurrentPoint = nextPoint;
		return ps;
	}

	void GcodeParser::handleMCode(float code, const vector<string> &args)
	{
		double spindleSpeed = parseCoord(args, 'S');
		if (!isnan(spindleSpeed))
		{
			mLastSpindleSpeed = spindleSpeed;
		}
	}

	PointSegment* GcodeParser::handleGCode(GCodeCommand* command, float code)
	{
		//cout << "GcodeParser: handleGCode" << code << args;
		vector<string> args = command->getArgs();

		PointSegment* ps;
		vec3 nextPoint = updatePointWithCommand(args, mCurrentPoint, mInAbsoluteMode);

		if (Util::floatsAreEqual(code,GCODE_RAPID))
		{
			ps=addLinearPointSegment(command, nextPoint, true);
		}
		else if (Util::floatsAreEqual(code,GCODE_LINEAR_INTERPOLATION))
		{
			ps=addLinearPointSegment(command, nextPoint, false);
		}
		else if (Util::floatsAreEqual(code,GCODE_STRAIGHT_PROBE))
		{
			ps=addLinearPointSegment(command, nextPoint, false);
		}
		else if (Util::floatsAreEqual(code,GCODE_ARC_MOVE_IJK))
		{
			ps=addArcPointSegment(command, nextPoint, true, args);
		}
		else if (Util::floatsAreEqual(code,GCODE_ARC_MOVE_RP))
		{
			ps=addArcPointSegment(command, nextPoint, false, args);
		}
		else if (Util::floatsAreEqual(code, GCODE_PLANE_XY))
		{
			mCurrentPlane = PointSegment::XY;
		}
		else if (Util::floatsAreEqual(code, GCODE_PLANE_ZX))
		{
			mCurrentPlane = PointSegment::ZX;
		}
		else if (Util::floatsAreEqual(code, GCODE_PLANE_YZ))
		{
			mCurrentPlane = PointSegment::YZ;
		}
		else if (Util::floatsAreEqual(code,GCODE_UNITS_INCHES))
		{
			mIsMetric = false;
		}
		else if (Util::floatsAreEqual(code, GCODE_UNITS_MM))
		{
			mIsMetric = true;
		}
		else if (Util::floatsAreEqual(code,GCODE_DISTANCE_ABSOLUTE))
		{
			mInAbsoluteMode = true;
		}
		else if (Util::floatsAreEqual(code,GCODE_DISTANCE_ABSOLUTE_IJK))
		{
			mInAbsoluteIJKMode = true;
		}
		else if (Util::floatsAreEqual(code,GCODE_DISTANCE_INCREMENTAL))
		{
			mInAbsoluteMode = false;
		}
		else if (Util::floatsAreEqual(code, GCODE_DISTANCE_INCREMENTAL_IJK))
		{
			mInAbsoluteIJKMode = false;
		}

		if  (
			 Util::floatsAreEqual(code, GCODE_RAPID) ||
			 Util::floatsAreEqual(code, GCODE_LINEAR_INTERPOLATION) ||
			 Util::floatsAreEqual(code, GCODE_ARC_MOVE_IJK) ||
			 Util::floatsAreEqual(code, GCODE_ARC_MOVE_RP) ||
			 Util::floatsAreEqual(code, GCODE_STRAIGHT_PROBE)
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

			// If this is enabled we need to parse the gcode as we go along.
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
		// Don't add them to the gcode parser since it is who expanded them.
		foreach (PointSegment* segment, psl)
		{
			//Point3d end = segment.point();
			vec3 *end = segment->getPointHandle();
			result.push_back(generateG1FromPoints(*start, *end, mInAbsoluteMode, mTruncateDecimalLength));
			start = segment->getPointHandle();
		}
		return result;
	}
	*/

	/**
	* Searches the command string for an 'f' and replaces the speed value
	* between the 'f' and the next space with a percentage of that speed.
	* In that way all speed values become a ratio of the provided speed
	* and don't get overridden with just a fixed speed.
	*/
	string GcodeParser::overrideSpeed(string command, double speed)
	{
		static QRegExp re("[Ff]([0-9.]+)");

		if (re.indexIn(command) != -1)
		{
			double speedMagnitude = (speed/100); // from percentage to magnitude
			command.replace(re, string("F%1").arg(re.cap(1).toDouble() * speedMagnitude));
		}
		return command;
	}

	/**
	* Searches the command string for an 'f' and replaces the speed value
	* between the 'f' and the next space with a percentage of that speed.
	* In that way all speed values become a ratio of the provided speed
	* and don't get overridden with just a fixed speed.
	*/
	GCodeCommand* GcodeParser::overrideSpeed(const GCodeCommand *command, double speed)
	{
		string cmd = command->getCommand();
		static QRegExp re("[Ff]([0-9.]+)");

		if (re.indexIn(cmd) != -1)
		{
			double speedMagnitude = (speed/100); // from percentage to magnitude
			cmd.replace(re, string("F%1").arg(re.cap(1).toDouble() * speedMagnitude));
		}
		// TODO - Leak Spin
		GCodeCommand* overriddenFR = new GCodeCommand(command);
		overriddenFR->setCommand(cmd);
		return overriddenFR;
	}

	/**
	* Removes any comments within parentheses or beginning with a semi-colon.
	*/
	string GcodeParser::removeComment(string command)
	{
		static QRegExp rx1("\\(+[^\\(]*\\)+");
		static QRegExp rx2(";.*");

		// Remove any comments within ( parentheses ) using regex "\([^\(]*\)"
		if (command.contains('(')) command.remove(rx1);

		// Remove any comment beginning with ';' using regex ";.*"
		if (command.contains(';')) command.remove(rx2);

		return command.trimmed();
	}

	/**
	* Searches for a comment in the input string and returns the first match.
	*/
	string GcodeParser::parseComment(const string command)
	{
		// REGEX: Find any comment, includes the comment characters:
		// "(?<=\()[^\(\)]*|(?<=\;)[^;]*"
		// "(?<=\\()[^\\(\\)]*|(?<=\\;)[^;]*"

		cout << "GcodeParser: Parsing comment in command " << command;
		static QRegExp re("(\\([^\\(\\)]*\\)|;[^;].*)");

		if (re.indexIn(command) != -1)
		{
			string cap = re.cap(1);
			cout << "GcodeParser: Found comment right here" << cap;
			return cap;
		}
		return "";
	}

	string GcodeParser::truncateDecimals(int length, string command)
	{
		static QRegExp re("(\\d*\\.\\d*)");
		int pos = 0;

		while ((pos = re.indexIn(command, pos)) != -1)
		{
			string newNum = string::number(re.cap(1).toDouble(), 'f', length);
			command = command.left(pos) + newNum + command.mid(pos + re.matchedLength());
			pos += newNum.length() + 1;
		}

		return command;
	}

	string GcodeParser::removeAllWhitespace(string command)
	{
		static QRegExp rx("\\s");

		return command.remove(rx);
	}

	vector<float> GcodeParser::parseCodes(const vector<string> &args, char code)
	{
		vector<float> l;

		foreach (string s, args)
		{
			if (s.length() > 0 && s[0].toUpper() == code)
			{
				l.push_back(s.mid(1).toFloat());
			}
		}

		return l;
	}

	vector<int> GcodeParser::parseGCodes(string command)
	{
		static QRegExp re("[Gg]0*(\\d+)");

		vector<int> codes;
		int pos = 0;

		while ((pos = re.indexIn(command, pos)) != -1)
		{
			codes.push_back(re.cap(1).toInt());
			pos += re.matchedLength();
		}

		return codes;
	}

	vector<int> GcodeParser::parseMCodes(string command)
	{
		static QRegExp re("[Mm]0*(\\d+)");

		vector<int> codes;
		int pos = 0;

		while ((pos = re.indexIn(command, pos)) != -1)
		{
			codes.push_back(re.cap(1).toInt());
			pos += re.matchedLength();
		}

		return codes;
	}

	/**
	* Update a point given the arguments of a command.
	*/
	vec3 GcodeParser::updatePointWithCommand(const string &command, const vec3 &initial, bool absoluteMode)
	{
		vector<string> l = splitCommand(command);
		return updatePointWithCommand(l, initial, absoluteMode);
	}

	/**
	* Update a point given the arguments of a command, using a pre-parsed list.
	*/
	vec3 GcodeParser::updatePointWithCommand(const vector<string> &commandArgs, const vec3 &initial,
															 bool absoluteMode)
	{
		double x = qQNaN();
		double y = qQNaN();
		double z = qQNaN();
		char c;

		for (int i = 0; i < commandArgs.length(); i++)
		{
			if (commandArgs.at(i).length() > 0)
			{
				c = commandArgs.at(i).at(0).toUpper().toLatin1();
				switch (c)
				{
				case 'X':
					x = commandArgs.at(i).mid(1).toDouble();;
					break;
				case 'Y':
					y = commandArgs.at(i).mid(1).toDouble();;
					break;
				case 'Z':
					z = commandArgs.at(i).mid(1).toDouble();;
					break;
				}
			}
		}

		return updatePointWithCommand(initial, x, y, z, absoluteMode);
	}

	/**
	* Update a point given the new coordinates.
	*/
	vec3 GcodeParser::updatePointWithCommand(const vec3 &initial, double x, double y, double z, bool absoluteMode)
	{
		vec3 newPoint(initial);// = vec3(qQNaN(),qQNaN(),qQNaN());

		if (absoluteMode)
		{
			if (!isnan(x)) newPoint.setX(x);
			if (!isnan(y)) newPoint.setY(y);
			if (!isnan(z)) newPoint.setZ(z);
		}
		else
		{
			if (!isnan(x)) newPoint.setX(initial.x() + x);
			if (!isnan(y)) newPoint.setY(initial.y() + y);
			if (!isnan(z)) newPoint.setZ(initial.z() + z);
		}

		return newPoint;
	}

	vec3 GcodeParser::updateCenterWithCommand(vector<string> commandArgs, vec3 initial, vec3 nextPoint, bool absoluteIJKMode, bool clockwise)
	{
		double i = qQNaN();
		double j = qQNaN();
		double k = qQNaN();
		double r = qQNaN();
		char c;

		foreach (string t, commandArgs)
		{
			if (t.length() > 0)
			{
				c = t[0].toUpper().toLatin1();
				switch (c)
				{
				case 'I':
					i = t.mid(1).toDouble();
					break;
				case 'J':
					j = t.mid(1).toDouble();
					break;
				case 'K':
					k = t.mid(1).toDouble();
					break;
				case 'R':
					r = t.mid(1).toDouble();
					break;
				}
			}
		}

		if (isnan(i) && isnan(j) && isnan(k))
		{
			return convertRToCenter(initial, nextPoint, r, absoluteIJKMode, clockwise);
		}

		return updatePointWithCommand(initial, i, j, k, absoluteIJKMode);
	}

	string GcodeParser::generateG1FromPoints(vec3 start, vec3 end, bool absoluteMode, int precision)
	{
		string sb("G1");

		if (absoluteMode)
		{
			if (!isnan(end.x())) sb.push_back("X" + string::number(end.x(), 'f', precision));
			if (!isnan(end.y())) sb.push_back("Y" + string::number(end.y(), 'f', precision));
			if (!isnan(end.z())) sb.push_back("Z" + string::number(end.z(), 'f', precision));
		}
		else
		{
			if (!isnan(end.x())) sb.push_back("X" + string::number(end.x() - start.x(), 'f', precision));
			if (!isnan(end.y())) sb.push_back("Y" + string::number(end.y() - start.y(), 'f', precision));
			if (!isnan(end.z())) sb.push_back("Z" + string::number(end.z() - start.z(), 'f', precision));
		}

		return sb;
	}

	/**
	* Splits a gcode command by each word/argument, doesn't care about spaces.
	* This command is about the same speed as the string.split(" ") command,
	* but might be a little faster using precompiled regex.
	*/
	vector<string> GcodeParser::splitCommand(const string &command)
	{
		vector<string> l;
		bool readNumeric = false;
		string sb;

		QByteArray ba(command.toLatin1());
		const char *cmd = ba.constData(); // Direct access to string data
		char c;

		for (int i = 0; i < command.length(); i++)
		{
			c = cmd[i];

			if (readNumeric && !isDigit(c) && c != '.')
			{
				readNumeric = false;
				l.push_back(sb);
				sb.clear();
				if (isLetter(c)) sb.push_back(c);
			}
			else if (isDigit(c) || c == '.' || c == '-')
			{
				sb.push_back(c);
				readNumeric = true;
			}
			else if (isLetter(c))
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
	double GcodeParser::parseCoord(vector<string> argList, char c)
	{
	//    int n = argList.length();

	//    for (int i = 0; i < n; i++) {
	//        if (argList[i].length() > 0 && argList[i][0].toUpper() == c) return argList[i].mid(1).toDouble();
	//    }

		foreach (string t, argList)
		{
			if (t.length() > 0 && t[0].toUpper() == c) return t.mid(1).toDouble();
		}
		return qQNaN();
	}

	//static public List<String> convertArcsToLines(Point3d start, Point3d end) {
	//    List<String> l = new ArrayList<String>();

	//    return l;
	//}

	vec3 GcodeParser::convertRToCenter(vec3 start, vec3 end, double rad, bool absoluteIJK, bool clockwise)
	{
		double radius = rad;
		vec3 center;

		double x = end.x() - start.x();
		double y = end.y() - start.y();

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

		double offsetX = 0.5 * (x - (y * height_x2_div_diameter));
		double offsetY = 0.5 * (y + (x * height_x2_div_diameter));

		if (!absoluteIJK)
		{
			center.setX(start.x() + offsetX);
			center.setY(start.y() + offsetY);
		}
		else
		{
			center.setX(offsetX);
			center.setY(offsetY);
		}

		return center;
	}

	/**
	* Return the angle in radians when going from start to end.
	*/
	double GcodeParser::getAngle(vec3 start, vec3 end)
	{
		double deltaX = end.x() - start.x();
		double deltaY = end.y() - start.y();

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

	double GcodeParser::calculateSweep(double startAngle, double endAngle, bool isCw)
	{
		double sweep;

		// Full circle
		if (Util::floatsAreEqual(startAngle,endAngle))
		{
			sweep = (M_PI * 2);
			// Arcs
		}
		else
		{
			// Account for full circles and end angles of 0/360
			if (Util::floatsAreEqual(endAngle,0))
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
	GcodeParser::generatePointsAlongArcBDring
	(
			PointSegment::planes plane, vec3 start, vec3 end,
			vec3 center, bool clockwise, double rad, double minArcLength,
			double arcPrecision, bool arcDegreeMode
	){
		double radius = rad;
		// Rotate vectors according to plane
		QMatrix4x4 m;
		m.setToIdentity();
		switch (plane)
		{
		case PointSegment::XY:
			break;
		case PointSegment::ZX:
			m.rotate(90, 1.0, 0.0, 0.0);
			break;
		case PointSegment::YZ:
			m.rotate(-90, 0.0, 1.0, 0.0);
			break;
		}
		start = m * start;
		end = m * end;
		center = m * center;

		// Check center
		if (isnan(center.length()))
		{
			return vector<vec3>();
		}

		// Calculate radius if necessary.
		if (Util::floatsAreEqual(radius,0))
		{
			radius = sqrt(pow((double)(start.x() - center.x()), 2.0) + pow((double)(end.y() - center.y()), 2.0));
		}

		double startAngle = getAngle(center, start);
		double endAngle = getAngle(center, end);
		double sweep = calculateSweep(startAngle, endAngle, clockwise);

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
			numPoints = qMax(1.0, sweep / (M_PI * arcPrecision / 180));
		}
		else
		{
			if (arcPrecision <= 0 && minArcLength > 0)
			{
				arcPrecision = minArcLength;
			}
			numPoints = (int)ceil(arcLength/arcPrecision);
		}

		return generatePointsAlongArcBDring(
			plane, start, end, center, clockwise,
			radius, startAngle, sweep, numPoints
		);
	}

	/**
	* Generates the points along an arc including the start and end points.
	*/
	vector<vec3>
	GcodeParser::generatePointsAlongArcBDring
	(
		PointSegment::planes plane, vec3 p1, vec3 p2,
		vec3 center, bool isCw, double radius, double startAngle,
		double sweep, int numPoints
	){
		// Prepare rotation matrix to restore plane
		QMatrix4x4 m;
		m.setToIdentity();
		switch (plane)
		{
		case PointSegment::XY:
			break;
		case PointSegment::ZX:
			m.rotate(-90, 1.0, 0.0, 0.0);
			break;
		case PointSegment::YZ:
			m.rotate(90, 0.0, 1.0, 0.0);
			break;
		}

		vec3 lineEnd(p2.x(), p2.y(), p1.z());
		vector<vec3> segments;
		double angle;

		// Calculate radius if necessary.
		if (radius == 0)
		{
			radius = sqrt(pow((double)(p1.x() - center.x()), 2.0) + pow((double)(p1.y() - center.y()), 2.0));
		}

		double zIncrement = (p2.z() - p1.z()) / numPoints;
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

			lineEnd.setX(cos(angle) * radius + center.x());
			lineEnd.setY(sin(angle) * radius + center.y());
			lineEnd.setZ(lineEnd.z() + zIncrement);

			segments.push_back(m * lineEnd);
		}

		segments.push_back(m * p2);

		return segments;
	}

	bool GcodeParser::isDigit(char c)
	{
		return c > 47 && c < 58;
	}

	bool GcodeParser::isLetter(char c)
	{
		return (c > 64 && c < 91) || (c > 96 && c < 123);
	}

	char GcodeParser::toUpper(char c)
	{
		return (c > 96 && c < 123) ? c - 32 : c;
	}

}
