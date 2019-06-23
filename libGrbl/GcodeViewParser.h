// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeViewParse.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include <QObject>
#include <QVector3D>
#include <QVector2D>
#include "LineSegment.h"
#include "GcodeParser.h"
#include "Utils/Util.h"

namespace Coconut {
class GcodeViewParser : public QObject
{
    Q_OBJECT
public:
    explicit GcodeViewParser(QObject *parent = nullptr);
    ~GcodeViewParser();

    QVector3D getMinimumExtremes() const ;
    QVector3D getMaximumExtremes() const;
    double getMinLength() const;
    QSize getResolution() const;
    QList<LineSegment*> getLineSegmentHandlesList() const;
    QVector<QList<int>> getLinesIndexes() const;
    void setLinesFromParser(GcodeParser* gp, double arcPrecision, bool arcDegreeMode);

    void reset();

signals:

public slots:

private:
    bool mAbsoluteMode;
    bool mAbsoluteIJK;

    // Parsed object
    QVector3D mMin, mMax;
    double mMinLength;
    QList<QSharedPointer<LineSegment>> mLines;
    QVector<QList<int>> mLineIndexes;

    // Parsing state.
    QVector3D mLastPoint;
    int mCurrentLine; // for assigning line numbers to segments.

    // Debug
    bool debug;
    void testExtremes(const QVector3D &p3d);
    void testExtremes(double x, double y, double z);
    void testLength(const QVector3D &start, const QVector3D &end);
};
}
