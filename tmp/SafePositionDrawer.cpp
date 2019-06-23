#include "SafePositionDrawer.h"

SafePositionDrawer::SafePositionDrawer()
{
    mLineWidth = 4;
    mPosition = QVector3D(0.0,0.0,0.0);
}

SafePositionDrawer::~SafePositionDrawer()
{

}

bool SafePositionDrawer::updateData()
{
    QVector4D green = QVector4D(0.0, 1.0, 0.0, 1.0);

    mLines.clear();
    mTriangles.clear();

    // Z-axis
    mTriangles.append({QVector3D(mPosition.x(),     mPosition.y(), mPosition.z()), green, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()+1, mPosition.y(), mPosition.z()+4),  green, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()-1, mPosition.y(), mPosition.z()+4),  green, QVector3D(sNan, sNan, sNan)});

    mTriangles.append({QVector3D(mPosition.x(), mPosition.y(), mPosition.z()), green, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x(), mPosition.y()+1, mPosition.z()+4),  green, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x(), mPosition.y()-1, mPosition.z()+4),  green, QVector3D(sNan, sNan, sNan)});

    return true;
}

QVector3D SafePositionDrawer::getSizes()
{
   return QVector3D();
}

QVector3D SafePositionDrawer::getMinimumExtremes()
{
   return QVector3D();
}

QVector3D SafePositionDrawer::getMaximumExtremes()
{
   return QVector3D();
}

int SafePositionDrawer::getVertexCount()
{
   return mLines.size();
}

QVector3D SafePositionDrawer::getPosition() const
{
    return mPosition;
}

void SafePositionDrawer::setPosition(const QVector3D& position)
{
    mPosition = position;
    update();
}
