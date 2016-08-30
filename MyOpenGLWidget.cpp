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
//    CreateCirclesBuffer();
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

void MyOpenGLWidget::SetFrame(int frame)
{
    m_Frame = frame;
    if (m_DrawCircles)
    {
//        CreateCirclesBuffer();
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

void MyOpenGLWidget::SetUseBuffers(bool useBuffers)
{
    m_UseBuffers = useBuffers;
    update();
}

void MyOpenGLWidget::SetZoom(int zoom)
{
    m_Zoom = 100/(float)zoom;
    update();
}

void MyOpenGLWidget::SetTrajectory(QVector<QVector<QVector3D>> traj)
{
    m_Traj = traj;
    CreateTrajBuffer();
//    CreateCirclesBuffer();
}

void MyOpenGLWidget::SetVertices(QVector<QVector<Vertex>> vertices)
{
    m_Vertices = vertices;
    CreateTrajBuffer();
//    CreateCirclesBuffer();
}

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{

}

void MyOpenGLWidget::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    m_Program = new QOpenGLShaderProgram();
    m_Program->bind();
    m_Program->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                            ":/shaders/points.vert");
    m_Program->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                            ":/shaders/points.frag");
    m_Program->link();
    m_ModelToWorld = m_Program->uniformLocation("modelToWorld");
    m_WorldToView = m_Program->uniformLocation("worldToView");
    m_Program->release();

    m_TrajBuffer.create();
    m_CircleBuffer.create();
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
    m_Projection.setToIdentity();
    m_Projection.perspective(qRadiansToDegrees(FOV)*m_Zoom, (float)w/(float)h,
                             m_Near, m_Far);
}

void MyOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(qRadiansToDegrees(FOV)*m_Zoom,
//                   (float)this->width()/(float)this->height(),
//                   m_Near,
//                   m_Far);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(m_Eye.x(),m_Eye.y(),m_Eye.z(),
//              m_Center.x(),m_Center.y(),m_Center.z(),
//              m_Up.x(),m_Up.y(),m_Up.z());

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
        QVector<Vertex> circleVertices = getCircleVertices(centers[i]);

        m_CircleBuffer.write(i*circleSize,
                             circleVertices.constData(),
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
    for (int i = 0; i < m_Atoms; ++i)
    {
        glDrawArrays(GL_TRIANGLE_FAN, i*CIRCLE_VERTICES, CIRCLE_VERTICES);
    }

    m_CircleBuffer.release();
    m_Program->release();
}

void MyOpenGLWidget::drawPaths()
{
    if(m_UseBuffers)
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

        glLineWidth(1.0f);
        for (int i = 0; i < m_Atoms; ++i)
        {
            glDrawArrays(GL_LINE_STRIP, i*m_TotalFrames, m_TotalFrames);
        }

        m_TrajBuffer.release();
        m_Program->release();
    }
    else
    {
        for (int i = 0 ; i < m_Atoms; ++i)
        {
            glBegin(GL_LINE_STRIP);
            for (int j = 0; j < m_TotalFrames; ++j)
            {
                QVector3D pos = m_Vertices[i][j].GetPosition();
                QVector3D colour = m_Vertices[i][j].GetColour();
                glColor3f(colour.x(), colour.y(), colour.z());
                glVertex3f(pos.x(), pos.y(), pos.z());
            }
            glEnd();
        }
    }

}

void MyOpenGLWidget::drawPoints()
{
    if (m_UseBuffers)
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

        m_Program->setUniformValue(m_WorldToView, m_Projection);
        m_Program->setUniformValue(m_ModelToWorld, m_Transform.ToMatrix());

        glPointSize(m_CircleRadius);

        glDrawArrays(GL_POINTS, 0, m_Atoms);

        m_TrajBuffer.release();
        m_Program->release();
    }
    else
    {
        glPointSize(m_CircleRadius);
        glBegin(GL_POINTS);
        for (int i = 0; i < m_Atoms; ++i)
        {
            QVector3D pos = m_Vertices[i][m_Frame].GetPosition();
            QVector3D colour = m_Vertices[i][m_Frame].GetColour();
            glColor3f(colour.x(), colour.y(), colour.z());
            glVertex3f(pos.x(), pos.y(), pos.z());
        }
        glEnd();
    }
}

QVector<QVector3D> MyOpenGLWidget::getCircleVertices(QVector3D center)
{
    QVector<QVector3D> vertices;
    vertices.append(center);
    float x = center.x();
    float y = center.y();
    float z = center.z();
    float scaling = 1.0/464.0;
    float halfR = m_CircleRadius*scaling/2.0;

    vertices.append(QVector3D(x, y + 2*halfR, z));
    vertices.append(QVector3D(x + halfR*SQRT_THREE, y + halfR, z));
    vertices.append(QVector3D(x + halfR*SQRT_THREE, y - halfR, z));
    vertices.append(QVector3D(x, y - 2*halfR, z));
    vertices.append(QVector3D(x - halfR*SQRT_THREE, y - halfR, z));
    vertices.append(QVector3D(x - halfR*SQRT_THREE, y + halfR, z));
    vertices.append(QVector3D(x, y + 2*halfR, z));

    return vertices;
}

QVector<Vertex> MyOpenGLWidget::getCircleVertices(Vertex center)
{
    QVector<Vertex> vertices;
    vertices.append(center);
    float x = center.GetPosition().x();
    float y = center.GetPosition().y();
    float z = center.GetPosition().z();
    float scaling = 1.0/464.0;
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
    float width = this->width();
    float height = this->height();
    float dx = (event->x() - m_LastX)/width;
    float dy = (event->y() - m_LastY)/height;

    QVector3D dir_x = m_Up;
    QVector3D temp = QVector3D::crossProduct(m_Up, m_Center - m_Eye);
    QVector3D dir_y = temp/temp.length();

    if (m_IsPanning)
    {
        float length = 2*(m_Eye - m_Center).length()*qTan(FOV/2);
        m_Center += m_Zoom*dir_y*dx*length*width/height
                  + m_Zoom*dir_x*dy*length;

    }
    else if (m_IsRotating)
    {
        m_Eye = rotatePoint(m_Eye, m_Center, dir_x, -dx*M_PI);
        m_Eye = rotatePoint(m_Eye, m_Center, dir_y, dy*M_PI);
        m_Up = rotatePoint(m_Center+m_Up, m_Center, dir_x, dy*M_PI) - m_Center;
        m_Up.normalize();
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

QVector3D MyOpenGLWidget::rotatePoint(QVector3D point,
                                      QVector3D origin,
                                      QVector3D direction,
                                      float angle)
{
    float sin = qSin(angle);
    float cos = qCos(angle);
    float val = 1 - cos;
    float x = direction.x();
    float y = direction.y();
    float z = direction.z();
    QMatrix4x4 rotMatrix = {x*x*val+cos,   x*y*val-x*sin, x*z*val+y*sin, 0,
                            y*x*val+x*sin, y*y*val+cos,   y*z*val-x*sin, 0,
                            z*x*val-y*sin, z*y*val+x*sin, z*z*val+cos,   0,
                            0,0,0,1};

    return origin + rotMatrix.map(point-origin);
}
