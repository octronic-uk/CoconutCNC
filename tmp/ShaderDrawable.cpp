//#define sNan qQNaN();

#include "ShaderDrawable.h"
#include <QtDebug>

#ifdef GLES
#include <GLES/gl.h>
#endif

ShaderDrawable::ShaderDrawable()
{
    //qDebug() << "ShaderDrawable: Constructing";
    mNeedsUpdateGeometry = true;
    mVisible = true;
    mLineWidth = 1.0f;
    mPointSize = 1.0f;
    mTexture = nullptr;
}

ShaderDrawable::~ShaderDrawable()
{
    //qDebug() << "ShaderDrawable: Destructing";
    if (!mVAO.isCreated()) mVAO.destroy();
    if (!mVBO.isCreated()) mVBO.destroy();
}

void ShaderDrawable::init()
{
    //qDebug() << "ShaderDrawable: init";
    // Init openGL functions
    initializeOpenGLFunctions();
    // Create buffers
    mVAO.create();
    mVBO.create();
}

void ShaderDrawable::update()
{
    mNeedsUpdateGeometry = true;
}

void ShaderDrawable::updateGeometry(QOpenGLShaderProgram *shaderProgram)
{
    //qDebug() << "ShaderDrawable: updateGeometry";

    // Init in context
    if (!mVBO.isCreated()) init();

    if (mVAO.isCreated())
    {
        // Prepare vao
        mVAO.bind();
    }

    // Prepare vbo
    mVBO.bind();

    // Update vertex buffer
    if (updateData())
    {
        // Fill vertices buffer
        QVector<VertexData> vertexData(mTriangles);
        vertexData += mLines;
        vertexData += mPoints;
        mVBO.allocate(vertexData.constData(), vertexData.count() * sizeof(VertexData));
    }
    else
    {
        //qDebug() << "ShaderDrawable: Releasing VBO";
        mVBO.release();
        if (mVAO.isCreated())
        {
            mVAO.release();
        }
        mNeedsUpdateGeometry = false;
        return;
    }

    if (mVAO.isCreated())
    {
        //qDebug() << "ShaderDrawable: VAO is created";
        // Offset for position
        int offset = 0;

        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = shaderProgram->attributeLocation("a_position");
        shaderProgram->enableAttributeArray(vertexLocation);
        shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Offset for color
        offset = sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex color data
        int color = shaderProgram->attributeLocation("a_color");
        shaderProgram->enableAttributeArray(color);
        shaderProgram->setAttributeBuffer(color, GL_FLOAT, offset, 4, sizeof(VertexData));

        // Offset for line start point
        offset += sizeof(QVector4D);

        // Tell OpenGL programmable pipeline how to locate vertex line start point
        int start = shaderProgram->attributeLocation("a_start");
        shaderProgram->enableAttributeArray(start);
        shaderProgram->setAttributeBuffer(start, GL_FLOAT, offset, 3, sizeof(VertexData));

        mVAO.release();
    }

    mVBO.release();

    mNeedsUpdateGeometry = false;
}

bool ShaderDrawable::updateData()
{
    // Test data
    mLines =
    {
        {QVector3D(0, 0, 0), QVector3D(1, 0, 0), QVector3D(sNan, 0, 0)},
        {QVector3D(10, 0, 0), QVector3D(1, 0, 0), QVector3D(sNan, 0, 0)},
        {QVector3D(0, 0, 0), QVector3D(0, 1, 0), QVector3D(sNan, 0, 0)},
        {QVector3D(0, 10, 0), QVector3D(0, 1, 0), QVector3D(sNan, 0, 0)},
        {QVector3D(0, 0, 0), QVector3D(0, 0, 1), QVector3D(sNan, 0, 0)},
        {QVector3D(0, 0, 10), QVector3D(0, 0, 1), QVector3D(sNan, 0, 0)}
    };

    return true;
}

bool ShaderDrawable::needsUpdateGeometry() const
{
    return mNeedsUpdateGeometry;
}

void ShaderDrawable::draw(QOpenGLShaderProgram *shaderProgram)
{
    if (!mVisible)
    {
        return;
    }

    if (mVAO.isCreated())
    {
        // Prepare vao
        mVAO.bind();
    }
    else
    {
        // Prepare vbo
        mVBO.bind();

        // Offset for position
        int offset = 0;

        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = shaderProgram->attributeLocation("a_position");
        shaderProgram->enableAttributeArray(vertexLocation);
        shaderProgram->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Offset for color
        offset = sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex color data
        int color = shaderProgram->attributeLocation("a_color");
        shaderProgram->enableAttributeArray(color);
        shaderProgram->setAttributeBuffer(color, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Offset for line start point
        offset += sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex line start point
        int start = shaderProgram->attributeLocation("a_start");
        shaderProgram->enableAttributeArray(start);
        shaderProgram->setAttributeBuffer(start, GL_FLOAT, offset, 3, sizeof(VertexData));
    }

    // TODO: Add triangles
    if (!mTriangles.isEmpty())
    {
        //qDebug() << "ShaderDrawable: Drawing triangles" << mTriangles.count();
        if (mTexture)
        {
            mTexture->bind();
            shaderProgram->setUniformValue("texture", 0);
        }
        glDrawArrays(GL_TRIANGLES, 0, mTriangles.count());
    }

    if (!mLines.isEmpty())
    {
        glLineWidth(mLineWidth);
        glDrawArrays(GL_LINES, mTriangles.count(), mLines.count());
    }

    if (!mPoints.isEmpty())
    {
        glDrawArrays(GL_POINTS, mTriangles.count() + mLines.count(), mPoints.count());
    }

    if (mVAO.isCreated())
    {
        mVAO.release();
    }
    else
    {
        mVBO.release();
    }
}

QVector3D ShaderDrawable::getSizes()
{
    return QVector3D(0, 0, 0);
}

QVector3D ShaderDrawable::getMinimumExtremes() const
{
    return QVector3D(0, 0, 0);
}

QVector3D ShaderDrawable::getMaximumExtremes() const
{
    return QVector3D(0, 0, 0);
}

int ShaderDrawable::getVertexCount()
{
    return mLines.count() + mPoints.count() + mTriangles.count();
}

float ShaderDrawable::lineWidth() const
{
    return mLineWidth;
}

void ShaderDrawable::setLineWidth(float lineWidth)
{
    mLineWidth = lineWidth;
}

bool ShaderDrawable::visible() const
{
    return mVisible;
}

void ShaderDrawable::setVisible(bool visible)
{
    mVisible = visible;
}

float ShaderDrawable::pointSize() const
{
    return mPointSize;
}

void ShaderDrawable::setPointSize(float pointSize)
{
    mPointSize = pointSize;
}

void ShaderDrawable::setNeedsUpdate()
{
   mNeedsUpdateGeometry = true;
}


