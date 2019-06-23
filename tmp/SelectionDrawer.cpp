#include "SelectionDrawer.h"

SelectionDrawer::SelectionDrawer()
{
    m_endPosition = QVector3D(sNan, sNan, sNan);
    mPointSize = 6.0;
    mLineWidth = 3;
    m_color = QColor("Purple");
}

bool SelectionDrawer::updateData()
{
    mPoints.clear();
    VertexData vertex;
    vertex.color = Util::colorToVector(m_color);
    vertex.position = m_endPosition;
    vertex.start = QVector3D(sNan, sNan, mPointSize);
    mPoints.append(vertex);

    return true;
}

QVector3D SelectionDrawer::endPosition() const
{
    return m_endPosition;
}

void SelectionDrawer::setEndPosition(const QVector3D &endPosition)
{
    m_endPosition = endPosition;
}

QColor SelectionDrawer::color() const
{
    return m_color;
}

void SelectionDrawer::setColor(const QColor &color)
{
    m_color = color;
}

QVector3D SelectionDrawer::startPosition() const
{
    return m_startPosition;
}

void SelectionDrawer::setStartPosition(const QVector3D &startPosition)
{
    m_startPosition = startPosition;
}
