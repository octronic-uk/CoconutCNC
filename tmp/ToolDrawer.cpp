// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include "ToolDrawer.h"
#include "Model/Settings/Sql/SqlSettingsModel.h"
#include "Model/Settings/ToolHolder/ToolHolderGeometry.h"

ToolDrawer::ToolDrawer()
    : mToolDiameter(4.0),
      mToolLength(30.0),
      mEndLength(0.0),
      mToolPosition(QVector3D(0, 0, 0)),
      mRotationAngle(0.0),
      mToolAngle(0.0),
      mColor(QColor("Orange")),
      mSettingsModelHandle(nullptr),
      mToolHandle(nullptr),
      mSpindleRotating(false),
      mSpindleSpeed(10.0f)

{
    qDebug() << "ToolDrawer: Constructing";
    mLineWidth= 3.0f;
}

ToolDrawer::~ToolDrawer()
{
    qDebug() << "ToolDrawer: Destructing";
}

void ToolDrawer::generateToolGeometry()
{
   //qDebug() << "ToolDrawer: Doing it the new way...";
   QVector4D color = Util::colorToVector(mColor);
   color.setW(1.0f);

   float currentZ = 0;

   for (ToolGeometry* geom : mToolHandle->getGeometryTableModelHandle()->getDataHandles())
   {
       /*qDebug() << "ToolDrawer: Doing it the new way with tool"
                << geom->getToolID()
                << "geom" << geom->getID();
                */

       int slices = geom->getFaces();
       float upperRadius = geom->getUpperDiameter()/2;
       float lowerRadius = geom->getLowerDiameter()/2;
       float height = geom->getHeight();
       float twoPi = 2 * static_cast<float>(M_PI);

       for(int i=0; i<slices; i++)
       {
           //qDebug() << "ToolDrawer: Making a slice" << i;

            float theta = ((twoPi / slices) * i ) + mRotationAngle;
            float nextTheta = ((twoPi / slices) * (i+1)) + mRotationAngle;

            //qDebug () << "ToolDrawer: theta " << theta;
            //qDebug () << "ToolDrawer: nextTheta " << nextTheta;
            /*vertex at top middle */
            QVector3D topMiddlePos(
                mToolPosition.x(),
                mToolPosition.y(),
                mToolPosition.z() + currentZ + height
            );
            VertexData topMiddle;
            topMiddle.color = color;
            topMiddle.position = topMiddlePos;
            topMiddle.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: topMiddle" << topMiddlePos;

            /*vertices at top edges of circle*/
            QVector3D topEdge1Pos(
                mToolPosition.x() + upperRadius*cos(theta),
                mToolPosition.y() + upperRadius*sin(theta),
                mToolPosition.z() + currentZ + height
            );
            VertexData topEdge1;
            topEdge1.color = color;
            topEdge1.position = topEdge1Pos;
            topEdge1.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: topEdge1" << topEdge1Pos;

            QVector3D topEdge2Pos(
                mToolPosition.x() + upperRadius*cos(nextTheta),
                mToolPosition.y() + upperRadius*sin(nextTheta),
                mToolPosition.z() + currentZ+height
            );
            VertexData topEdge2;
            topEdge2.color = color;
            topEdge2.position = topEdge2Pos;
            topEdge2.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: topEdge2" << topEdge2Pos;

            /*vertex at bottom middle */
            QVector3D bottomMiddlePos(
                mToolPosition.x(),
                mToolPosition.y(),
                mToolPosition.z() + currentZ
            );
            VertexData bottomMiddle;
            bottomMiddle.color = color;
            bottomMiddle.position = bottomMiddlePos;
            bottomMiddle.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: bottomMiddle" << bottomMiddlePos;

            /*vertices at bottom edges of circle*/
            QVector3D bottomEdge1Pos(
                mToolPosition.x() + lowerRadius*cos(theta),
                mToolPosition.y() + lowerRadius*sin(theta),
                mToolPosition.z()  + currentZ
            );

            VertexData bottomEdge1;
            bottomEdge1.color = color;
            bottomEdge1.position = bottomEdge1Pos;
            bottomEdge1.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: bottomEdge1" << bottomEdge1Pos;

            QVector3D bottomEdge2Pos(
                mToolPosition.x() + lowerRadius*cos(nextTheta),
                mToolPosition.y() + lowerRadius*sin(nextTheta),
                mToolPosition.z() + currentZ
            );

            VertexData bottomEdge2;
            bottomEdge2.color = color;
            bottomEdge2.position = bottomEdge2Pos;
            bottomEdge2.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: bottomEdge2" << bottomEdge2Pos;

            // Top
            mTriangles.append(topMiddle);
            mTriangles.append(topEdge1);
            mTriangles.append(topEdge2);

            // Side1
            mTriangles.append(topEdge2);
            mTriangles.append(topEdge1);
            mTriangles.append(bottomEdge1);

            // Side2
            mTriangles.append(bottomEdge1);
            mTriangles.append(bottomEdge2);
            mTriangles.append(topEdge2);

            // Bottom
            mTriangles.append(bottomMiddle);
            mTriangles.append(bottomEdge1);
            mTriangles.append(bottomEdge2);

        }

        currentZ += height;
        /* Fade away
        color.setX(color.x() * 1.5f);
        color.setY(color.y() * 1.5f);
        color.setZ(color.z() * 1.5f);
        color.setW(color.w() * 1.2f);
        */

   }
}

void ToolDrawer::generateToolHolderGeometry()
{
    ToolHolder* holder = mSettingsModelHandle->getToolHolderByID(mToolHandle->getToolHolderID());
   if (!holder)
   {
        qDebug() << "ToolDrawer: No tool holder assigned";
       return;
   }

   //qDebug() << "ToolDrawer: Doing it the new way...";
   QVector4D color(0.1f,0.1f,0.1f,1.0f);

   float currentZ = mToolHandle->getTotalHeight();

   for (ToolHolderGeometry* geom : holder->getGeometryTableModelHandle()->getDataHandles())
   {
       /*qDebug() << "ToolDrawer: Doing it the new way with tool"
                << geom->getToolID()
                << "geom" << geom->getID();
                */

       int slices = geom->getFaces();
       float upperRadius = geom->getUpperDiameter()/2;
       float lowerRadius = geom->getLowerDiameter()/2;
       float height = geom->getHeight();


       for(int i=0; i<slices; i++)
       {
           //qDebug() << "ToolDrawer: Making a slice" << i;

            float theta = (2 * static_cast<float>(M_PI) * i / slices) + mRotationAngle;
            float nextTheta = (2 * static_cast<float>(M_PI) * (i+1) / slices) + mRotationAngle;

            /*vertex at top middle */
            QVector3D topMiddlePos(
                mToolPosition.x(),
                mToolPosition.y(),
                mToolPosition.z() + currentZ + height
            );
            VertexData topMiddle;
            topMiddle.color = color;
            topMiddle.position = topMiddlePos;
            topMiddle.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: topMiddle" << topMiddlePos;

            /*vertices at top edges of circle*/
            QVector3D topEdge1Pos(
                mToolPosition.x() + upperRadius*cos(theta),
                mToolPosition.y() + upperRadius*sin(theta),
                mToolPosition.z() + currentZ + height
            );
            VertexData topEdge1;
            topEdge1.color = color;
            topEdge1.position = topEdge1Pos;
            topEdge1.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: topEdge1" << topEdge1Pos;

            QVector3D topEdge2Pos(
                mToolPosition.x() + upperRadius*cos(nextTheta),
                mToolPosition.y() + upperRadius*sin(nextTheta),
                mToolPosition.z() + currentZ+height
            );
            VertexData topEdge2;
            topEdge2.color = color;
            topEdge2.position = topEdge2Pos;
            topEdge2.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: topEdge2" << topEdge2Pos;

            /*vertex at bottom middle */
            QVector3D bottomMiddlePos(
                mToolPosition.x(),
                mToolPosition.y(),
                mToolPosition.z() + currentZ
            );
            VertexData bottomMiddle;
            bottomMiddle.color = color;
            bottomMiddle.position = bottomMiddlePos;
            bottomMiddle.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: bottomMiddle" << bottomMiddlePos;

            /*vertices at bottom edges of circle*/
            QVector3D bottomEdge1Pos(
                mToolPosition.x() + lowerRadius*cos(theta),
                mToolPosition.y() + lowerRadius*sin(theta),
                mToolPosition.z()  + currentZ
            );

            VertexData bottomEdge1;
            bottomEdge1.color = color;
            bottomEdge1.position = bottomEdge1Pos;
            bottomEdge1.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: bottomEdge1" << bottomEdge1Pos;

            QVector3D bottomEdge2Pos(
                mToolPosition.x() + lowerRadius*cos(nextTheta),
                mToolPosition.y() + lowerRadius*sin(nextTheta),
                mToolPosition.z() + currentZ
            );

            VertexData bottomEdge2;
            bottomEdge2.color = color;
            bottomEdge2.position = bottomEdge2Pos;
            bottomEdge2.start = QVector3D(0,0,0);
            //qDebug() << "ToolDrawer: bottomEdge2" << bottomEdge2Pos;

            // Top
            mTriangles.append(topMiddle);
            mTriangles.append(topEdge1);
            mTriangles.append(topEdge2);

            // Side1
            mTriangles.append(topEdge1);
            mTriangles.append(topEdge2);
            mTriangles.append(bottomEdge2);

            // Side2
            mTriangles.append(bottomEdge2);
            mTriangles.append(topEdge1);
            mTriangles.append(bottomEdge1);

            // Bottom
            mTriangles.append(bottomMiddle);
            mTriangles.append(bottomEdge1);
            mTriangles.append(bottomEdge2);

        }

        currentZ += height;

        // Fade away
        color.setX(color.x() * 1.75f);
        color.setY(color.y() * 1.75f);
        color.setZ(color.z() * 1.75f);
        //color.setW(color.w() * 0.75f); // Looks ugly, fix vertex windings
   }
}

bool ToolDrawer::updateData()
{
    mLines.clear();
    mPoints.clear();
    mTriangles.clear();

    if (!mToolHandle)
    {
        return true;
    }

    rotate();
    generateToolGeometry();
    generateToolHolderGeometry();

    return true;
}

SqlSettingsModel* ToolDrawer::getSettingsModelHandle() const
{
    return mSettingsModelHandle;
}

void ToolDrawer::setSettingsModelHandle(SqlSettingsModel* settingsModelHandle)
{
    mSettingsModelHandle = settingsModelHandle;
}

void ToolDrawer::setSpindleRotating(bool rotating)
{
   mSpindleRotating = rotating;
}

void ToolDrawer::setSpindleSpeed(float speed)
{
   mSpindleSpeed = speed;
}

Tool* ToolDrawer::getToolHandle() const
{
    return mToolHandle;
}

void ToolDrawer::setToolHandle(Tool* toolHandle)
{
    mToolHandle = toolHandle;
}

QColor ToolDrawer::color() const
{
    return mColor;
}

void ToolDrawer::setColor(const QColor &color)
{
    mColor = color;
}


float ToolDrawer::toolDiameter() const
{
    return mToolDiameter;
}

void ToolDrawer::setToolDiameter(float toolDiameter)
{
    if (mToolDiameter != toolDiameter)
    {
        mToolDiameter = toolDiameter;
        update();
    }
}

float ToolDrawer::toolLength() const
{
    return mToolLength;
}

void ToolDrawer::setToolLength(float toolLength)
{
    if (mToolLength != toolLength)
    {
        mToolLength = toolLength;
        update();
    }
}
QVector3D ToolDrawer::toolPosition() const
{
    return mToolPosition;
}

void ToolDrawer::setToolPosition(const QVector3D &toolPosition)
{
    if (mToolPosition != toolPosition)
    {
        mToolPosition = toolPosition;
        update();
    }
}

void ToolDrawer::rotate()
{
    mRotationAngle = normalizeAngle(mRotationAngle + mSpindleSpeed);
}

float ToolDrawer::toolAngle() const
{
    return mToolAngle;
}

void ToolDrawer::setToolAngle(float toolAngle)
{
    if (mToolAngle != toolAngle)
    {
        mToolAngle = toolAngle;
        mEndLength =
                mToolAngle > 0 && mToolAngle < 180.0f ?
                    mToolDiameter / 2 / tan(mToolAngle / 180.0f * static_cast<float>(M_PI) / 2) :
                    0;
        if (mToolLength < mEndLength)
        {
            mToolLength = mEndLength;
        }
        update();
    }
}

float ToolDrawer::normalizeAngle(float angle)
{
    while (angle < 0)
    {
        angle += 360;
    }

    while (angle > 360)
    {
        angle -= 360;
    }

    return angle;
}

bool ToolDrawer::needsUpdateGeometry() const
{
    return mNeedsUpdateGeometry || mSpindleRotating;
}
