#pragma once

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include "Utils/Util.h"

struct VertexData
{
    QVector3D position;
    QVector4D color;
    QVector3D start;
};

class ShaderDrawable : protected QOpenGLFunctions
{
public:
    explicit ShaderDrawable();
    virtual ~ShaderDrawable();
    void update();
    void draw(QOpenGLShaderProgram *shaderProgram);

    bool needsUpdateGeometry() const;
    void updateGeometry(QOpenGLShaderProgram *shaderProgram = nullptr);

    virtual QVector3D getSizes();
    virtual QVector3D getMinimumExtremes() const;
    virtual QVector3D getMaximumExtremes() const;
    virtual int getVertexCount();

    float lineWidth() const;
    void setLineWidth(float lineWidth);

    bool visible() const;
    void setVisible(bool visible);

    float pointSize() const;
    void setPointSize(float pointSize);

    void setNeedsUpdate();

signals:

public slots:

protected:
    float mLineWidth;
    float mPointSize;
    bool mVisible;
    QVector<VertexData> mLines;
    QVector<VertexData> mPoints;
    QVector<VertexData> mTriangles;
    QOpenGLTexture *mTexture;

    QOpenGLBuffer mVBO; // Protected for direct vbo access

    virtual bool updateData();
    void init();
    bool mNeedsUpdateGeometry;

private:
    QOpenGLVertexArrayObject mVAO;
};
