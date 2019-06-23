#include "OriginDrawer.h"

OriginDrawer::OriginDrawer()
{
    mLineWidth = 4;
    mPosition = QVector3D(0.0,0.0,0.0);
}

OriginDrawer::~OriginDrawer()
{

}

bool OriginDrawer::updateData()
{
    QVector4D red = QVector4D(1.0, 0.0, 0.0, 1.0);
    QVector4D green = QVector4D(0.0, 1.0, 0.0, 1.0);
    QVector4D blue = QVector4D(0.0, 0.0, 1.0, 1.0);
    QVector4D white = QVector4D(1.0,1.0,1.0,1.0);

    mLines.clear();
    mTriangles.clear();

    // X-axis
    mLines.append({QVector3D(mPosition.x(),    mPosition.y(),     mPosition.z()), red, QVector3D(sNan, sNan, sNan)});
    mLines.append({QVector3D(mPosition.x()+8,  mPosition.y(),     mPosition.z()), red, QVector3D(sNan, sNan, sNan)});

    mTriangles.append({QVector3D(mPosition.x()+10, mPosition.y(),     mPosition.z()), red, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()+8,  mPosition.y()+0.5, mPosition.z()), red, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()+8,  mPosition.y()-0.5, mPosition.z()), red, QVector3D(sNan, sNan, sNan)});

    mTriangles.append({QVector3D(mPosition.x()+10, mPosition.y(), mPosition.z()),     red, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()+8,  mPosition.y(), mPosition.z()+0.5), red, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()+8,  mPosition.y(), mPosition.z()-0.5), red, QVector3D(sNan, sNan, sNan)});

    // Y-axis
    mLines.append({QVector3D(mPosition.x(), mPosition.y(),   mPosition.z()), green, QVector3D(sNan, sNan, sNan)});
    mLines.append({QVector3D(mPosition.x(), mPosition.y()+9, mPosition.z()), green, QVector3D(sNan, sNan, sNan)});

    mTriangles.append({QVector3D(mPosition.x(),     mPosition.y()+10, mPosition.z()), green, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()+0.5, mPosition.y()+8,  mPosition.z()), green, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()-0.5, mPosition.y()+8,  mPosition.z()), green, QVector3D(sNan, sNan, sNan)});

    mTriangles.append({QVector3D(mPosition.x(), mPosition.y()+10, mPosition.z()),     green, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x(), mPosition.y()+8, mPosition.z()+0.5),  green, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x(), mPosition.y()+8, mPosition.z()-0.5),  green, QVector3D(sNan, sNan, sNan)});

    // Z-axis
    mLines.append({QVector3D(mPosition.x(), mPosition.y(), mPosition.z()),    blue, QVector3D(sNan, sNan, sNan)});
    mLines.append({QVector3D(mPosition.x(), mPosition.y(), mPosition.z()+9),  blue, QVector3D(sNan, sNan, sNan)});

    mTriangles.append({QVector3D(mPosition.x(),     mPosition.y(), mPosition.z()+10), blue, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()+0.5, mPosition.y(), mPosition.z()+8),  blue, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x()-0.5, mPosition.y(), mPosition.z()+8),  blue, QVector3D(sNan, sNan, sNan)});

    mTriangles.append({QVector3D(mPosition.x(), mPosition.y(), mPosition.z()+10), blue, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x(), mPosition.y()+0.5, mPosition.z()+8),  blue, QVector3D(sNan, sNan, sNan)});
    mTriangles.append({QVector3D(mPosition.x(), mPosition.y()-0.5, mPosition.z()+8),  blue, QVector3D(sNan, sNan, sNan)});

    return true;
}

QVector3D OriginDrawer::getSizes()
{
   return QVector3D();
}

QVector3D OriginDrawer::getMinimumExtremes()
{
   return QVector3D();
}

QVector3D OriginDrawer::getMaximumExtremes()
{
   return QVector3D();
}

int OriginDrawer::getVertexCount()
{
   return mLines.size();
}

QVector3D OriginDrawer::getPosition() const
{
    return mPosition;
}

void OriginDrawer::setPosition(const QVector3D& position)
{
    mPosition = position;
    update();
}
