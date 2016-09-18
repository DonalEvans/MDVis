#include "MyOpenGLWidget.h"
#include "GL/glu.h"
#include "math.h"
#include <QMouseEvent>
#include <QtMath>

void MyOpenGLWidget::SetAmbientValue(int ambientValue)
{
    m_AmbientValue = (float)ambientValue/100;
    update();
}

void MyOpenGLWidget::SetCircleRadius(int radius)
{
    m_CircleRadius = (float)radius/RADIUS_SCALING;
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
    update();
}

void MyOpenGLWidget::SetMaxPathLength(int percentage)
{
    if (percentage > m_MinPathLength*HUNDRED)
    {
        m_MaxPathLength = (float)percentage/HUNDRED;
        update();
    }
}

void MyOpenGLWidget::SetMinPathLength(int percentage)
{
    if (percentage < m_MaxPathLength*HUNDRED)
    {
        m_MinPathLength = (float)percentage/HUNDRED;
        update();
    }
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
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    m_CircleRadius = 1/RADIUS_SCALING;

    m_PathProgram = new QOpenGLShaderProgram();
    m_PathProgram->bind();
    m_PathProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                            ":/shaders/paths.vert");
    m_PathProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                            ":/shaders/paths.frag");
    m_PathProgram->link();
    m_PathProgram->release();

    m_PointProgram = new QOpenGLShaderProgram();
    m_PointProgram->bind();
    m_PointProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                            ":/shaders/points.vert");
    m_PointProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                            ":/shaders/points.frag");
    m_PointProgram->link();
    m_PointProgram->release();

    m_TrajBuffer.create();
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
        drawPoints();
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

void MyOpenGLWidget::drawPaths()
{
    m_PathProgram->bind();
    m_TrajBuffer.bind();

    m_PathProgram->enableAttributeArray(0);
    m_PathProgram->enableAttributeArray(1);
    int skippedAtoms = m_Atoms*m_MinPathLength;
    int filterOffset = m_TotalFrames*skippedAtoms*Vertex::Stride();
    m_PathProgram->setAttributeBuffer(0, GL_FLOAT,
                                  filterOffset + Vertex::PositionOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride());
    m_PathProgram->setAttributeBuffer(1, GL_FLOAT,
                                  filterOffset + Vertex::ColourOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride());

    m_PathProgram->setUniformValue(m_PathProgram->uniformLocation("modelToWorld"),
                                   m_Transform.ToMatrix());
    m_PathProgram->setUniformValue(m_PathProgram->uniformLocation("worldToCamera"),
                                   m_Camera.ToMatrix());
    m_PathProgram->setUniformValue(m_PathProgram->uniformLocation("cameraToView"),
                                   m_Projection);

    glLineWidth(1.0f);
    for (int i = 0; i < m_Atoms*(m_MaxPathLength-m_MinPathLength); ++i)
    {
        glDrawArrays(GL_LINE_STRIP, i*m_TotalFrames, m_TotalFrames);
    }

    m_TrajBuffer.release();
    m_PathProgram->release();

}

void MyOpenGLWidget::drawPoints()
{
    m_PointProgram->bind();
    m_TrajBuffer.bind();

    int frameOffset = m_Frame*Vertex::Stride();
    int skippedAtoms = m_Atoms*m_MinPathLength;
    int filterOffset = m_TotalFrames*skippedAtoms*Vertex::Stride();

    m_PointProgram->enableAttributeArray(0);
    m_PointProgram->enableAttributeArray(1);
    m_PointProgram->setAttributeBuffer(0, GL_FLOAT,
                                  filterOffset + frameOffset + Vertex::PositionOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride()*m_TotalFrames);
    m_PointProgram->setAttributeBuffer(1, GL_FLOAT,
                                  filterOffset + frameOffset + Vertex::ColourOffset(),
                                  Vertex::TUPLE_SIZE,
                                  Vertex::Stride()*m_TotalFrames);

    m_PointProgram->setUniformValue(m_PointProgram->uniformLocation("modelToWorld"),
                                    m_Transform.ToMatrix());
    m_PointProgram->setUniformValue(m_PointProgram->uniformLocation("worldToCamera"),
                                    m_Camera.ToMatrix());
    m_PointProgram->setUniformValue(m_PointProgram->uniformLocation("cameraToView"),
                                    m_Projection);
    m_PointProgram->setUniformValue(m_PointProgram->uniformLocation("lighting"),
                                    m_LightingMatrix.ToMatrix());
    m_PointProgram->setUniformValue(m_PointProgram->uniformLocation("ambient"),
                                    m_AmbientValue);

    glPointSize(m_CircleRadius/m_Zoom);

    glDrawArrays(GL_POINTS, 0, m_Atoms*(m_MaxPathLength-m_MinPathLength));

    m_TrajBuffer.release();
    m_PointProgram->release();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx = event->x() - m_LastX;
    float dy = event->y() - m_LastY;

    float transSpeed = TRANS_SPEED*m_Zoom;

    if (m_IsPanning)
    {
        QVector3D trans;
        if(event->modifiers() == Qt::ShiftModifier)
        {
            trans += dy*m_Camera.Forward();
        }
        else
        {
            trans += dx*m_Camera.Right();
            trans += dy*m_Camera.Up();
        }
        m_Camera.Translate(trans*transSpeed);
    }
    else if (m_IsRotating)
    {
        if(event->modifiers() == Qt::ShiftModifier)
        {
            float centerX = this->width()/2;
            float centerY = this->height()/2;
            bool isLeft = ((m_LastX - centerX)*(event->y() - centerY)
                           - (m_LastY - centerY)*(event->x() - centerX)) > 0;
            int direction = 1;
            if (!isLeft)
            {
                direction = -1;
            }
            float ammount = qSqrt(dy*dy+dx*dx)/2;
            QQuaternion axisRot = QQuaternion::fromAxisAndAngle(m_Camera.Forward(),
                                                                ROT_SPEED*
                                                                direction*
                                                                ammount);
            m_Camera.Rotate(axisRot);
        }
        else if(event->modifiers() == Qt::ControlModifier)
        {
            QQuaternion upRot = QQuaternion::fromAxisAndAngle(m_LightingMatrix.Up(),
                                                              -ROT_SPEED*dx);
            QQuaternion rightRot = QQuaternion::fromAxisAndAngle(m_LightingMatrix.Right(),
                                                                 ROT_SPEED*dy);

            m_LightingMatrix.Rotate(upRot);
            m_LightingMatrix.Rotate(rightRot);

        }
        else
        {
            QQuaternion upRot = QQuaternion::fromAxisAndAngle(m_Camera.Up(),
                                                              -ROT_SPEED*dx);
            QQuaternion rightRot = QQuaternion::fromAxisAndAngle(m_Camera.Right(),
                                                                 ROT_SPEED*dy);

            m_Camera.Rotate(upRot);
            m_Camera.Rotate(rightRot);
        }
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

void MyOpenGLWidget::ResetLighting()
{
    m_LightingMatrix.ResetView();
    update();
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
    QVector3D eye(0,0,-m_Far/2);
    QVector3D center(0,0,0);
    QVector3D up(0,1,0);
    defaultView.lookAt(eye,center,up);
    m_Camera.SetDefaultView(defaultView);
    m_LightingMatrix.SetDefaultView(defaultView);
}

void MyOpenGLWidget::PrintMatrix(QMatrix4x4 matrix)
{
    QTextStream out(stdout);
    out << matrix(0,0) << " " << matrix(0,1) << " " << matrix(0,2) << " " << matrix(0,3) << endl;
    out << matrix(1,0) << " " << matrix(1,1) << " " << matrix(1,2) << " " << matrix(1,3) << endl;
    out << matrix(2,0) << " " << matrix(2,1) << " " << matrix(2,2) << " " << matrix(2,3) << endl;
    out << matrix(3,0) << " " << matrix(3,1) << " " << matrix(3,2) << " " << matrix(3,3) << endl;
}
