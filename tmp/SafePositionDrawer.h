#pragma once
#include "ShaderDrawable.h"

class SafePositionDrawer : public ShaderDrawable
{
public:
    SafePositionDrawer();
    virtual ~SafePositionDrawer() override;

    QVector3D getSizes() override;
    QVector3D getMinimumExtremes();
    QVector3D getMaximumExtremes();
    int getVertexCount() override;

    QVector3D getPosition() const;
    void setPosition(const QVector3D& position);

protected:
    bool updateData() override;

private:
    QVector3D mPosition;
};
