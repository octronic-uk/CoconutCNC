// This file is a part of "CoconutCNC" application.
// This file was originally ported from "GcodeViewParse.java" class
// of "Universal GcodeSender" application written by Will Winder
// (https://github.com/winder/Universal-G-Code-Sender)

// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include <QDebug>
#include "GcodeViewParser.h"

namespace Coconut {
GcodeViewParser::GcodeViewParser(QObject *parent) :
    QObject(parent)
{
    mAbsoluteMode = true;
    mAbsoluteIJK = false;
    mCurrentLine = 0;
    debug = true;

    mMin = QVector3D(qQNaN(), qQNaN(), qQNaN());
    mMax = QVector3D(qQNaN(), qQNaN(), qQNaN());

    mMinLength = qQNaN();
}

GcodeViewParser::~GcodeViewParser()
{
}

QVector3D GcodeViewParser::getMinimumExtremes() const
{
    return mMin;
}

QVector3D GcodeViewParser::getMaximumExtremes() const
{
    return mMax;
}

void GcodeViewParser::testExtremes(const QVector3D &p3d)
{
    testExtremes(p3d.x(), p3d.y(), p3d.z());
}

void GcodeViewParser::testExtremes(double x, double y, double z)
{
    mMin.setX(Util::nMin(mMin.x(), x));
    mMin.setY(Util::nMin(mMin.y(), y));
    mMin.setZ(Util::nMin(mMin.z(), z));

    mMax.setX(Util::nMax(mMax.x(), x));
    mMax.setY(Util::nMax(mMax.y(), y));
    mMax.setZ(Util::nMax(mMax.z(), z));
}

void GcodeViewParser::testLength(const QVector3D &start, const QVector3D &end)
{
    double length = (start - end).length();
    if (!qIsNaN(length) && length != 0)
    {
        mMinLength = qIsNaN(mMinLength) ?
        length :
        qMin<double>(mMinLength, length);
    }
}

QList<LineSegment*> GcodeViewParser::getLineSegmentHandlesList() const
{
    QList<LineSegment*> handles;
    for (auto ls : mLines) handles.append(ls.data());
    return handles;
}

void GcodeViewParser::reset()
{
    mLines.clear();
    mLineIndexes.clear();
    mCurrentLine = 0;
    mMin = QVector3D(qQNaN(), qQNaN(), qQNaN());
    mMax = QVector3D(qQNaN(), qQNaN(), qQNaN());
    mMinLength = qQNaN();
}

double GcodeViewParser::getMinLength() const
{
    return mMinLength;
}

QSize GcodeViewParser::getResolution() const
{
    return QSize
    (
        ((mMax.x() - mMin.x()) / mMinLength) + 1,
        ((mMax.y() - mMin.y()) / mMinLength) + 1
    );
}

void GcodeViewParser::setLinesFromParser
(GcodeParser *gp, double arcPrecision, bool arcDegreeMode)
{
    // Remove old
    mLines.clear();

    QList<PointSegment*> psl = gp->getPointSegmentHandlesList();
    qDebug() << "GcodeViewParser: Point Segments" << psl.count();
    // For a line segment list ALL arcs must be converted to lines.
    double minArcLength = 0.1;
    //double length;

    QVector3D *start, *end;
    start = nullptr;
    end = nullptr;

    // Prepare segments indexes
    mLineIndexes.resize(psl.count());

    int lineIndex = 0;
    foreach (PointSegment *segment, psl)
    {
        PointSegment *ps = segment;
        bool isMetric = ps->isMetric();
        ps->convertToMetric();

        end = ps->getPointHandle();

        // start is null for the first iteration.
        if (start != nullptr)
        {
            // Expand arc for graphics.
            if (ps->isArc())
            {
                QList<QVector3D> points =
                    GcodeParser::generatePointsAlongArcBDring
                    (
                        ps->plane(),
                        *start, *end, ps->center(),
                        ps->isClockwise(), ps->getRadius(),
                        minArcLength, arcPrecision, arcDegreeMode
                    );
                // Create line segments from points.
                if (points.length() > 0)
                {
                    QVector3D startPoint = *start;
                    foreach (QVector3D nextPoint, points)
                    {
                        if (nextPoint == startPoint) continue;
                        auto ls = QSharedPointer<LineSegment>::create(startPoint, nextPoint, lineIndex);
                        ls->setIsArc(ps->isArc());
                        ls->setIsClockwise(ps->isClockwise());
                        ls->setPlane(ps->plane());
                        ls->setIsRapidMovement(ps->isRapidMovement());
                        ls->setIsZMovement(ps->isZMovement());
                        ls->setIsMetric(isMetric);
                        ls->setIsAbsolute(ps->isAbsolute());
                        ls->setSpeed(ps->getSpeed());
                        ls->setSpindleSpeed(ps->getSpindleSpeed());
                        ls->setDwell(ps->getDwell());
                        this->testExtremes(nextPoint);
                        mLines.append(ls);
                        mLineIndexes[ps->getLineNumber()].append(mLines.count() - 1);
                        startPoint = nextPoint;
                    }
                    lineIndex++;
                }
            // Line
            } else {
                auto ls = QSharedPointer<LineSegment>::create(*start, *end, lineIndex++);
                ls->setIsArc(ps->isArc());
                ls->setIsRapidMovement(ps->isRapidMovement());
                ls->setIsZMovement(ps->isZMovement());
                ls->setIsMetric(isMetric);
                ls->setIsAbsolute(ps->isAbsolute());
                ls->setSpeed(ps->getSpeed());
                ls->setSpindleSpeed(ps->getSpindleSpeed());
                ls->setDwell(ps->getDwell());
                this->testExtremes(*end);
                this->testLength(*start, *end);
                mLines.append(ls);
                mLineIndexes[ps->getLineNumber()].append(mLines.count() - 1);
            }
        }
        start = end;
    }
}

QVector<QList<int>> GcodeViewParser::getLinesIndexes() const
{
    return mLineIndexes;
}

}
