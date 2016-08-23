/**
 * @file MyOpenGLWidget.h
 * @author Donal Evans
 * @date 25 Jul 2016
 * @brief This class provides OpenGL functionalty for drawing to the main window.
 *
 * Detailed description goes here.
 */

#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include "Vertex.h"
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class MyOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    /**
     * @brief Setter for the trajectory data that is to be used in drawing.
     * @param traj A 2-dimensional QVector containing a QVector of 3D
     * coordinates for each time step, for each atom.
     */
    void SetTrajectory(QVector<QVector<QVector3D>> traj);

    void SetVertices(QVector<QVector<Vertex>> vertices);

    /**
     * @brief Constructor
     * @param parent The parent widget to which this object belongs.
     */
    MyOpenGLWidget(QWidget* parent);

    /**
     * @brief Loads the vertices required to draw circles into the GPU memory
     * as a buffer.
     */
    void CreateCirclesBuffer();

    /**
     * @brief Loads the vertices required to draw points and paths into the 
     * GPU memory as a buffer.
     */
    void CreateTrajBuffer();

public slots:
    /**
     * @brief Setter for the radius of the circles to be drawn.
     * @param radius The radius to be used.
     */
    void SetCircleRadius(int radius);

    /**
     * @brief Setter for whether or not circles will be drawn.
     * @param draw If true circles will be drawn, if false, they will not.
     */
    void SetDrawCircles(bool draw);
    
    /**
     * @brief Setter for whether or not paths will be drawn.
     * @param draw If true paths will be drawn, if false, they will not.
     */
    void SetDrawPaths(bool draw);

    /**
     * @brief Setter for whether or not points will be drawn.
     * @param draw If true points will be drawn, if false, they will not.
     */
    void SetDrawPoints(bool draw);

    /**
     * @brief Setter for which frame of data will be drawn.
     * @param frame The frame number that will be drawn.
     */
    void SetFrame(int frame);

    void SetZoom(int zoom);


protected:
    /**
     * @brief This function sets up the OpenGL environment and initializes
     * the shader programs.
     */
    void initializeGL();

    /**
     * @brief This function handles all the painting to be done by the class.
     */
    void paintGL();

    /**
     * @brief This function handles resizing of the drawing surface.
     * @param w The new width of the drawing surface.
     * @param h The new height of the drawing surface.
     */
    void resizeGL(int w, int h);

private:

    /**
     * @brief Uses the vertex data stored in the circle buffer to draw atom 
     * positions as circles to the drawing surface.
     */
    void drawCircles();

    /**
     * @brief Uses the vertex data stored in the trajectory buffer to draw
     * atom paths to the drawing surface.
     */
    void drawPaths();

    /**
     * @brief Uses the vertex data stored in the trajectory buffer to draw
     * atom positions as points to the drawing surface.
     */
    void drawPoints();

    void drawPoints2();

    /**
     * @brief Given the center of a circle, generates 12 3D coordinates 
     * representing points around the circumference.
     * @param center The 3D coordinates of the center of the circle.
     * @return A QVector of 3D coordinates representing the center of the
     * circle followed by 12 points around the circumference.
     */
    QVector<QVector3D> getCircleVertices(QVector3D center);

    QVector<Vertex> getCircleVertices(Vertex center);

    /**
     * @brief The buffer in which vertices required to draw circles are stored.
     */
    QOpenGLBuffer m_CircleBuffer;
    
    /**
     * @brief The shader program used for drawing atom paths.
     */
    QOpenGLShaderProgram* m_PathProgram;
    
    /**
     * @brief The shader program used for drawing atom positions at points.
     */
    QOpenGLShaderProgram* m_PointProgram;
    
    /**
     * @brief The buffer in which vertices used for drawing points and paths
     * are stored.
     */
    QOpenGLBuffer m_TrajBuffer;
    
    /**
     * @brief The number of atoms currently being drawn.
     */
    int m_Atoms;
    
    /**
     * @brief The radius of circles to be drawn.
     */
    float m_CircleRadius = 1.0;
    
    /**
     * @brief Flag determining if circles are to be drawn.
     */
    bool m_DrawCircles = false;
    
    /**
     * @brief Flag determining if paths are to be drawn.
     */
    bool m_DrawPaths = false;
    
    /**
     * @brief Flag determining if postition points are to be drawn.
     */
    bool m_DrawPoints = false;
    
    /**
     * @brief The frame of data for which positions are being drawn.
     */
    int m_Frame = 0;
    
    /**
     * @brief The total number of frames in the data.
     */
    int m_TotalFrames;
    
    /**
     * @brief The trajectory verctors for each atom.
     */
    QVector<QVector<QVector3D>> m_Traj;

    QVector<QVector<Vertex>> m_Vertices;

    float m_Zoom;
    
    /**
     * @brief The number of vertices retured by @getCircleVertices.
     */
    const int CIRCLE_VERTICES = 8;
    
    /**
     * @brief The square root of 3, used in calculating circle vertices.
     */
    const float SQRT_THREE = 1.732;
    
    /**
     * @brief The number of spatial dimensions being used.
     */
    const int TUPLE_SIZE_3D = 3;
};

#endif // MYOPENGLWIDGET_H
