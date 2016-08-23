#include "MyOpenGLWidget.h"
#include "GL/glu.h"

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
        CreateCirclesBuffer();
    }
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
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_COLOR_MATERIAL);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    m_PointProgram = new QOpenGLShaderProgram();
    m_PointProgram->bind();
    m_PointProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                            ":/shaders/points.vert");
    m_PointProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                            ":/shaders/points.frag");
    m_PointProgram->link();
    m_PointProgram->release();

    m_PathProgram = new QOpenGLShaderProgram();
    m_PathProgram->bind();
    m_PathProgram->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/paths.vert");
    m_PathProgram->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/paths.frag");
    m_PathProgram->link();
    m_PathProgram->release();
    m_TrajBuffer.create();
    m_CircleBuffer.create();
}

void MyOpenGLWidget::resizeGL(int w, int h)
{

}

void MyOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glFrustum(0,116,0,116,0,20);
//    glFrustum(-0.1,0.1,-0.1,0.1,0,2);
    gluPerspective(90*m_Zoom,(float)this->width()/(float)this->height(),0,2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//    gluLookAt(58,0,5,58,58,5,0,0,1);
    gluLookAt(0,-2,0.5, 0,0,0.5, 0,0,1);

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
//    QOpenGLWidget::makeCurrent();

//    m_Atoms = m_Traj.length();
//    m_TotalFrames = m_Traj[0].length();

//    m_TrajBuffer.bind();
//    m_TrajBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    m_TrajBuffer.allocate(sizeof(m_Traj[0][0])*m_Atoms*m_TotalFrames);

//    int offset = sizeof(m_Traj[0][0])*m_TotalFrames;

//    for (int i = 0; i < m_Atoms; ++i)
//    {
//        m_TrajBuffer.write(offset*i, m_Traj[i].constData(), offset);
//    }

//    m_TrajBuffer.release();
//    QOpenGLWidget::doneCurrent();

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
    m_PointProgram->bind();
    m_CircleBuffer.bind();

    m_PointProgram->enableAttributeArray(0);
    m_PointProgram->enableAttributeArray(1);
    m_PointProgram->setAttributeBuffer(0, GL_FLOAT,
                                       Vertex::PositionOffset(),
                                       Vertex::TUPLE_SIZE,
                                       Vertex::Stride());
    m_PointProgram->setAttributeBuffer(1, GL_FLOAT,
                                       Vertex::ColourOffset(),
                                       Vertex::TUPLE_SIZE,
                                       Vertex::Stride());
    for (int i = 0; i < m_Atoms; ++i)
    {
        glDrawArrays(GL_TRIANGLE_FAN, i*CIRCLE_VERTICES, CIRCLE_VERTICES);
    }

    m_CircleBuffer.release();
    m_PointProgram->release();
}

void MyOpenGLWidget::drawPaths()
{
//    m_PathProgram->bind();
//    m_TrajBuffer.bind();

//    m_PathProgram->enableAttributeArray(0);
//    m_PathProgram->setAttributeBuffer(0, GL_FLOAT, 0, TUPLE_SIZE_3D, 0);

//    glLineWidth(1.0f);
//    for (int i = 0; i < m_Atoms; ++i)
//    {
//        glDrawArrays(GL_LINE_STRIP, i*m_TotalFrames, m_TotalFrames);
//    }

//    m_TrajBuffer.release();
//    m_PathProgram->release();

    m_PathProgram->bind();
    m_TrajBuffer.bind();

    m_PathProgram->enableAttributeArray(0);
    m_PathProgram->enableAttributeArray(1);
    m_PathProgram->setAttributeBuffer(0, GL_FLOAT,
                                      Vertex::PositionOffset(),
                                      Vertex::TUPLE_SIZE,
                                      Vertex::Stride());
    m_PathProgram->setAttributeBuffer(1, GL_FLOAT,
                                      Vertex::ColourOffset(),
                                      Vertex::TUPLE_SIZE,
                                      Vertex::Stride());


    glLineWidth(1.0f);
    for (int i = 0; i < m_Atoms; ++i)
    {
        glDrawArrays(GL_LINE_STRIP, i*m_TotalFrames, m_TotalFrames);
    }

    m_TrajBuffer.release();
    m_PathProgram->release();

}

void MyOpenGLWidget::drawPoints()
{
//    m_PointProgram->bind();
//    m_TrajBuffer.bind();

//    m_PointProgram->enableAttributeArray(0);
//    m_PointProgram->setAttributeBuffer(0, GL_FLOAT,
//                                       m_Frame*sizeof(QVector3D),
//                                       TUPLE_SIZE_3D,
//                                       sizeof(QVector3D)*m_TotalFrames);

//    glPointSize(m_CircleRadius);
//    glDrawArrays(GL_POINTS, 0, m_Atoms);

//    m_TrajBuffer.release();
//    m_PointProgram->release();

    m_PointProgram->bind();
    m_TrajBuffer.bind();

    int frameOffset = m_Frame*Vertex::Stride();
    m_PointProgram->enableAttributeArray(0);
    m_PointProgram->enableAttributeArray(1);
    m_PointProgram->setAttributeBuffer(0, GL_FLOAT,
                                       frameOffset + Vertex::PositionOffset(),
                                       Vertex::TUPLE_SIZE,
                                       Vertex::Stride()*m_TotalFrames);
    m_PointProgram->setAttributeBuffer(1, GL_FLOAT,
                                       frameOffset + Vertex::ColourOffset(),
                                       Vertex::TUPLE_SIZE,
                                       Vertex::Stride()*m_TotalFrames);

    glPointSize(m_CircleRadius);
    glDrawArrays(GL_POINTS, 0, m_Atoms);

    m_TrajBuffer.release();
    m_PointProgram->release();
}

void MyOpenGLWidget::drawPoints2()
{
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

