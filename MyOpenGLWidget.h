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

    /**
     * @brief Setter for the Vertex data that is to be used in drawing.
     * @param vertices A 2-dimensional QVector containing @Vertex objects for
     * each time step, for each atom.
     */
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

    /**
     * @brief Setter for whether or not OpenGL buffers will be used to draw.
     * @param useBuffers True if OpenGL buffers are to be used to draw.
     */
    void SetUseBuffers(bool useBuffers);

    /**
     * @brief Setter for the zoom level.
     * @param zoom The percentage zoom level as an integer.
     */
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
     * @brief Setter for the mouse x position value on mouse click.
     * @param LastX The x position of the mouse on mouse click.
     */
    void setLastX(float LastX);

    /**
     * @brief Setter for the mouse y position value on mouse click.
     * @param LastY The y position of the mouse on mouse click.
     */
    void setLastY(float LastY);

    /**
     * @brief Setter for if panning is occurring.
     * @param panning
     */
    void setPan(bool panning);

    /**
     * @brief Setter for if rotation is occurring.
     * @param rotating
     */
    void setRotate(bool rotating);

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

    /**
     * @brief Given the center of a circle, generates 8 3D coordinates
     * representing points required to draw a circle using GL_TRIANGLE_FAN.
     * @param center The 3D coordinates of the center of the circle.
     * @return A QVector of 3D coordinates representing the center of the
     * circle followed by 7 points around the circumference.
     */
    QVector<QVector3D> getCircleVertices(QVector3D center);

    /**
     * @brief Given the center of a circle, generates 8 @Vertex objects
     * representing points required to draw a circle using GL_TRIANGLE_FAN.
     * @param center The @Vertex representing the center of the circle.
     * @return A QVector of @Vertex objects representing the center of the
     * circle followed by 7 points around the circumference.
     */
    QVector<Vertex> getCircleVertices(Vertex center);

    /**
     * @brief Handles behaviour on mouse movement.
     * @param event
     */
    virtual void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief Handles behaviour on mouse button press.
     * @param event
     */
    virtual void mousePressEvent(QMouseEvent *event);

    /**
     * @brief Handles behaviour on mouse button release.
     * @param event
     */
    virtual void mouseReleaseEvent(QMouseEvent *event);

    /**
     * @brief Rotates a point about an axis fixed in space.
     * @param point The point to be rotated in 3D coordinates.
     * @param origin The origin of the space in which the point lies.
     * @param direction The vector about which the point will be rotated.
     * @param angle The angle by which the point will be rotated.
     * @return A QVector3D representing the new coordinates of the point.
     */
    QVector3D rotatePoint(QVector3D point,
                          QVector3D origin,
                          QVector3D direction,
                          float angle);
    
    /**
     * @brief The number of atoms currently being drawn.
     */
    int m_Atoms;

    /**
     * @brief The vector representing the center position in gluLookat().
     */
    QVector3D m_Center = {58,58,5};

    /**
     * @brief The buffer in which vertices required to draw circles are stored.
     */
    QOpenGLBuffer m_CircleBuffer;
    
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
     * @brief The vector representing the eye position in gluLookat().
     */
    QVector3D m_Eye = {58,58,-120};

    /**
     * @brief The far clipping plane.
     */
    float m_Far = 2;

    /**
     * @brief The frame of data for which positions are being drawn.
     */
    int m_Frame = 0;

    /**
     * @brief The X position of the mouse at the beginning of a movement event.
     */
    float m_LastX;

    /**
     * @brief The Y position of the mouse at the beginning of a movement event.
     */
    float m_LastY;

    /**
     * @brief The near clipping plane.
     */
    float m_Near = 0;

    /**
     * @brief True if panning is occurring, false otherwise.
     */
    bool m_IsPanning;

    /**
     * @brief The shader program used for drawing atom paths.
     */
    QOpenGLShaderProgram* m_PathProgram;

    /**
     * @brief The shader program used for drawing atom positions at points.
     */
    QOpenGLShaderProgram* m_PointProgram;

    /**
     * @brief True if rotation is occurring, false otherwise.
     */
    bool m_IsRotating;
    
    /**
     * @brief The total number of frames in the data.
     */
    int m_TotalFrames;
    
    /**
     * @brief The trajectory verctors for each atom.
     */
    QVector<QVector<QVector3D>> m_Traj;

    /**
     * @brief The buffer in which vertices used for drawing points and paths
     * are stored.
     */
    QOpenGLBuffer m_TrajBuffer;

    /**
     * @brief The vector indicating the up direction in gluLookat().
     */
    QVector3D m_Up = {0,1,0};

    /**
     * @brief Determines how objects will be drawn; either using OpenGL buffers
     * or reading in vertex data every time PaintGL is called.
     */
    bool m_UseBuffers = false;

    /**
     * @brief The @Vertex objects for each @Atom at each time step of the data.
     */
    QVector<QVector<Vertex>> m_Vertices;

    /**
     * @brief The zoom to be applied to the image, as a fraction.
     */
    float m_Zoom = 1.0;
    
    /**
     * @brief The number of vertices retured by @getCircleVertices().
     */
    const int CIRCLE_VERTICES = 8;

    /**
     * @brief The FOV used in gluPerspective() in radians.
     */
    const float FOV = 0.88;
    
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
