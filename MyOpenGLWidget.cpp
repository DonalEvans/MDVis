#include "MyOpenGLWidget.h"
#include "GL/glu.h"
#include "math.h"
#include <QMouseEvent>
#include <QtMath>

void MyOpenGLWidget::SetCircleRadius(int radius)
{
    m_CircleRadius = radius;
    if (m_DrawCircles)
    {
        CreateCirclesBuffer();
    }
    update();
}

void MyOpenGLWidget::SetDrawCircles(bool draw)
{
    m_DrawCircles = draw;
    CreateCirclesBuffer();
    update();
}

void MyOpenGLWidget::SetDrawPaths(bool draw)
{
    m_DrawPaths = draw;
    update();
}

void MyOpenGLWidget::SetDrawPoints(bool draw)
{
    m_DrawPoints = draw;
    update();
}

float MyOpenGLWidget::getFar()
{
    return m_Far;
}

void MyOpenGLWidget::setFar(float newFar)
{
    m_Far = newFar;
}

void MyOpenGLWidget::SetFrame(int frame)
{
    m_Frame = frame;
    if (m_DrawCircles)
    {
        CreateCirclesBuffer();
    }
    update();
}

void MyOpenGLWidget::setLastY(float LastY)
{
    m_LastY = LastY;
}

void MyOpenGLWidget::setLastX(float LastX)
{
    m_LastX = LastX;
}

void MyOpenGLWidget::setPan(bool panning)
{
    m_IsPanning = panning;
}

void MyOpenGLWidget::setRotate(bool rotating)
{
    m_IsRotating = rotating;
}

QVector<QVector<Vertex>>& MyOpenGLWidget::GetVerticesRef()
{
    return m_Vertices;
}

void MyOpenGLWidget::SetVertices(QVector<QVector<Vertex>> vertices)
{
    m_Vertices = vertices;
    CreateTrajBuffer();
    CreateCirclesBuffer();
}

void MyOpenGLWidget::SetZoom(float zoom)
{
    m_Zoom = zoom;
    update();
}

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{

}

void MyOpenGLWidget::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    m_Program = new QOpenGLShaderProgram();
    m_Program->bind();
    m_Program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                            ":/shaders/points.vert");
    m_Program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                            ":/shaders/points.frag");
    m_Program->link();
    m_ModelToWorld = m_Program->uniformLocation("modelToWorld");
    m_WorldToCamera = m_Program->uniformLocation("worldToCamera");
    m_CameraToView = m_Program->uniformLocation("cameraToView");
    m_Program->release();

    m_TrajBuffer.create();
    m_CircleBuffer.create();
}

void MyOpenGLWidget::resizeGL(int w, int h)
{

}

void MyOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Projection.setToIdentity();
    m_Projection.perspective(qRadiansToDegrees(FOV)*m_Zoom,
                             (float)this->width()/(float)this->height(),
                             m_Near, m_Far);
//    m_Projection.ortho(-116,0,-116,0,0,1000);

    if(m_DrawPaths)
    {
        drawPaths();
    }
    if(m_DrawPoints)
    {
        if(m_DrawCircles)
        {
            drawCircles();
        }
        else
        {
            drawPoints();
        }
    }
}

void MyOpenGLWidget::AddVertices(QVector<Vertex> vertices)
{
    m_Vertices.append(vertices);
}

void MyOpenGLWidget::ClearData()
{
    m_Vertices.clear();
    m_Vertices.squeeze();
    m_TrajBuffer.destroy();
    m_TrajBuffer.create();
    m_CircleBuffer.destroy();
    m_CircleBuffer.create();
}

void MyOpenGLWidget::CreateCirclesBuffer()
{
    QOpenGLWidget::makeCurrent();

    QVector<Vertex> centers;
    for (int i = 0; i < m_Atoms; ++i)
    {
        centers.append(m_Vertices[i][m_Frame]);
    }
    m_CircleBuffer.bind();
    m_CircleBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int circleSize = sizeof(Vertex)*CIRCLE_VERTICES;
    m_CircleBuffer.allocate(circleSize*m_Atoms);
    for (int i = 0; i < m_Atoms; ++i)
    {
        m_CircleBuffer.write(i*circleSize,
                             getCircleVertices(centers[i]).constData(),
                             circleSize);
    }
    m_CircleBuffer.release();
    QOpenGLWidget::doneCurrent();
}

void MyOpenGLWidget::CreateTrajBuffer()
{
    QOpenGLWidget::makeCurrent();

    m_Atoms = m_Vertices.length();
    m_TotalFrames = m_Vertices[0].length();

    m_TrajBuffer.bind();
    m_TrajBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_TrajBuffer.allocate(sizeof(m_Vertices[0][0])*m_Atoms*m_TotalFrames);

    int offset = sizeof(m_Vertices[0][0])*m_TotalFrames;

    for (int i = 0; i < m_Atoms; ++i)
    {
        m_TrajBuffer.write(offset*i, m_Vertices[i].constData(), offset);
    }

    m_TrajBuffer.release();
    QOpenGLWidget::doneCurrent();
}

void MyOpenGLWidget::drawCircles()
{
    m_Program->bind();
    m_CircleBuffer.bind();

    m_Program->enableAttributeArray(0);
    m_Program->enableAttributeArray(1);
    m_Program->setAttributeBuffer(0, GL_FLOAT,
                                  Vertex::PositionOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride());
    m_Program->setAttributeBuffer(1, GL_FLOAT,
                                  Vertex::ColourOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride());

    m_Program->setUniformValue(m_ModelToWorld, m_Transform.ToMatrix());
    m_Program->setUniformValue(m_WorldToCamera, m_Camera.ToMatrix());
    m_Program->setUniformValue(m_CameraToView, m_Projection);

    for (int i = 0; i < m_Atoms; ++i)
    {
        glDrawArrays(GL_TRIANGLE_FAN, i*CIRCLE_VERTICES, CIRCLE_VERTICES);
    }

    m_CircleBuffer.release();
    m_Program->release();
}

void MyOpenGLWidget::drawPaths()
{
    m_Program->bind();
    m_TrajBuffer.bind();

    m_Program->enableAttributeArray(0);
    m_Program->enableAttributeArray(1);
    m_Program->setAttributeBuffer(0, GL_FLOAT,
                                  Vertex::PositionOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride());
    m_Program->setAttributeBuffer(1, GL_FLOAT,
                                  Vertex::ColourOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride());

    m_Program->setUniformValue(m_ModelToWorld, m_Transform.ToMatrix());
    m_Program->setUniformValue(m_WorldToCamera, m_Camera.ToMatrix());
    m_Program->setUniformValue(m_CameraToView, m_Projection);

    glLineWidth(1.0f);
    for (int i = 0; i < m_Atoms; ++i)
    {
        glDrawArrays(GL_LINE_STRIP, i*m_TotalFrames, m_TotalFrames);
    }

    m_TrajBuffer.release();
    m_Program->release();

}

void MyOpenGLWidget::drawPoints()
{
    m_Program->bind();
    m_TrajBuffer.bind();

    int frameOffset = m_Frame*Vertex::Stride();

    m_Program->enableAttributeArray(0);
    m_Program->enableAttributeArray(1);
    m_Program->setAttributeBuffer(0, GL_FLOAT,
                                  frameOffset + Vertex::PositionOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride()*m_TotalFrames);
    m_Program->setAttributeBuffer(1, GL_FLOAT,
                                  frameOffset + Vertex::ColourOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride()*m_TotalFrames);

    m_Program->setUniformValue(m_ModelToWorld, m_Transform.ToMatrix());
    m_Program->setUniformValue(m_WorldToCamera, m_Camera.ToMatrix());
    m_Program->setUniformValue(m_CameraToView, m_Projection);

    glPointSize(m_CircleRadius);

    glDrawArrays(GL_POINTS, 0, m_Atoms);

    m_TrajBuffer.release();
    m_Program->release();
}

QVector<Vertex> MyOpenGLWidget::getCircleVertices(Vertex center)
{
    QVector<Vertex> vertices;
    vertices.append(center);
    float x = center.GetPosition().x();
    float y = center.GetPosition().y();
    float z = center.GetPosition().z();
    float scaling = 1.0/50.0;
    float halfR = m_CircleRadius*scaling/2.0;

    QVector3D colour = center.GetColour();

    vertices.append(Vertex(QVector3D(x, y + 2*halfR, z),
                           colour));
    vertices.append(Vertex(QVector3D(x + halfR*SQRT_THREE, y + halfR, z),
                           colour));
    vertices.append(Vertex(QVector3D(x + halfR*SQRT_THREE, y - halfR, z),
                           colour));
    vertices.append(Vertex(QVector3D(x, y - 2*halfR, z),
                           colour));
    vertices.append(Vertex(QVector3D(x - halfR*SQRT_THREE, y - halfR, z),
                           colour));
    vertices.append(Vertex(QVector3D(x - halfR*SQRT_THREE, y + halfR, z),
                           colour));
    vertices.append(Vertex(QVector3D(x, y + 2*halfR, z),
                           colour));

    return vertices;
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx = event->x() - m_LastX;
    float dy = event->y() - m_LastY;

    float transSpeed = TRANS_SPEED*m_Zoom;
    float rotSpeed = ROT_SPEED;

    if (m_IsPanning)
    {
        QVector3D trans;
        trans -= dx*m_Camera.Right();
        trans += dy*m_Camera.Up();
        m_Camera.Translate(trans*transSpeed);

    }
    else if (m_IsRotating)
    {
        QQuaternion upRot = QQuaternion::fromAxisAndAngle(m_Camera.Up(),
                                                          -rotSpeed*dx);
        QQuaternion rightRot = QQuaternion::fromAxisAndAngle(m_Camera.Right(),
                                                             -rotSpeed*dy);

        m_Camera.Rotate(upRot);
        m_Camera.Rotate(rightRot);
    }

    update();

    setLastX(event->x());
    setLastY(event->y());

}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    setLastX(event->x());
    setLastY(event->y());
    if (event->button() == Qt::LeftButton)
    {
        setPan(true);
    }
    else if (event->button() == Qt::RightButton)
    {
        setRotate(true);
    }
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    setPan(false);
    setRotate(false);

    update();
}

void MyOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() < 0 && m_Zoom < ZOOM_SPEED)
    {
        SetZoom(m_Zoom*ZOOM_SPEED);
    }
    else if (event->delta() > 0)
    {
        SetZoom(m_Zoom/ZOOM_SPEED);
    }
}

void MyOpenGLWidget::ResetView()
{
    m_Camera.ResetView();
    SetZoom(1.0);
}

void MyOpenGLWidget::SetBoundingBox(QVector3D box)
{
    m_Transform.ResetRotation();
    m_Transform.ResetTranslation();
    m_Transform.Translate(-box/2);

    setFar(box.x()*3);

    QMatrix4x4 defaultView;
    QVector3D eye(0,0,m_Far/2);
    QVector3D center(0,0,0);
    QVector3D up(0,1,0);
    defaultView.lookAt(eye,center,up);
    m_Camera.SetDefaultView(defaultView);
    ResetView();
}
