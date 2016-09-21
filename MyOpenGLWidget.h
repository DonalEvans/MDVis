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
#include "Transform3D.h"
#include "Camera3D.h"
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class MyOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    /**
     * @brief Getter for a reference to the Vertex vector.
     * @return A reference to the Vertex vector.
     */
    QVector<QVector<Vertex>>& GetVerticesRef();

    /**
     * @brief Setter for the Vertex data that is to be used in drawing.
     * @param vertices A 2-dimensional QVector containing @Vertex objects for
     * each time step, for each atom.
     */
    void SetVertices(QVector<QVector<Vertex>> vertices);

    /**
     * @brief Setter for the zoom level.
     * @param zoom The zoom factor as a float.
     */
    void SetZoom(float zoom);

    /**
     * @brief Constructor
     * @param parent The parent widget to which this object belongs.
     */
    MyOpenGLWidget(QWidget* parent);

    /**
     * @brief Appends a QVector of @Vertex objects to @m_Vertices.
     * @param vertices The QVector of @Vertex objects to be appended.
     */
    void AddVertices(QVector<Vertex> vertices);

    /**
     * @brief Clears all data currently stored in this object.
     */
    void ClearData();

    /**
     * @brief Loads the vertices required to draw points and paths into the 
     * GPU memory as a buffer.
     */
    void CreateTrajBuffer();

    /**
     * @brief Convenience function for printint the contents of a 4x4 matrix.
     * @param matrix The QMatrix4x4 to be printed.
     */
    void PrintMatrix(QMatrix4x4 matrix);

    /**
     * @brief Sets the size of the bounding box for the data, allowing
     * initial camera position to be set.
     * @param box a QVector3D represening the boundaries of the simulation
     * space.
     */
    void SetBoundingBox(QVector3D box);

public slots:
    /**
     * @brief Resets the position of the diffuse lighting source to the default
     * view.
     */
    void ResetLighting();

    /**
     * @brief Resets the camera view to the default view.
     */
    void ResetView();

    /**
     * @brief Setter for the intensity of the ambient lighting.
     * @param ambientValue The intensity of the ambient lighting, as an integer
     * from 0 - 100.
     */
    void SetAmbientValue(int ambientValue);

    /**
     * @brief Setter for the radius of the circles to be drawn.
     * @param radius The radius to be used.
     */
    void SetCircleRadius(int radius);
    
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
     * @brief Sets the maximum path length to be drawn, as a percentage of the
     * total path length range.
     * @param percentage The maxmimum path length to be drawn, as a percentage
     * of the total path length.
     */
    void SetMaxPathLength(int percentage);

    /**
     * @brief Sets the minimum path length to be drawn, as a percentage of the
     * total path length range.
     * @param percentage The minmimum path length to be drawn, as a percentage
     * of the total path length.
     */
    void SetMinPathLength(int percentage);

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
     * @brief Getter for the far flipping plane.
     * @return The distance from the camera to teh far clipping plane
     * as a float.
     */
    float getFar();

    /**
     * @brief Setter for the far clipping plane.
     * @param newFar The distance from the camera to teh far clipping plane
     * as a float.
     */
    void setFar(float newFar);

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
     * @param panning True if panning, false otherwise.
     */
    void setPan(bool panning);

    /**
     * @brief Setter for if rotation is occurring.
     * @param rotating True if rotating, false otherwise.
     */
    void setRotate(bool rotating);

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
     * @brief Handles behaviour on mouse movement.
     * @param event The triggering QMouseEvent.
     */
    virtual void mouseMoveEvent(QMouseEvent *event);

    /**
     * @brief Handles behaviour on mouse button press.
     * @param event The triggering QMouseEvent.
     */
    virtual void mousePressEvent(QMouseEvent *event);

    /**
     * @brief Handles behaviour on mouse button release.
     * @param event The triggering QMouseEvent.
     */
    virtual void mouseReleaseEvent(QMouseEvent *event);

    /**
     * @brief Handles behaviour on mouse wheel scroll.
     * @param event The triggering QWheelEvent.
     */
    virtual void wheelEvent(QWheelEvent *event);

    /**
     * @brief The intensity of the ambient lighting, as a float.
     */
    float m_AmbientValue = 0.3;

    /**
     * @brief The number of atoms currently being drawn.
     */
    int m_Atoms;

    /**
     * @brief The camera object.
     */
    Camera3D m_Camera;

    /**
     * @brief The uniform location within the shader files of the camera to
     * view transformation matrix.
     */
    int m_CameraToView;
    
    /**
     * @brief The radius of circles to be drawn, as a float.
     */
    float m_CircleRadius;

    /**
     * @brief Matrix describing the default camera view.
     */
    QMatrix4x4 m_DefaultView;
    
    /**
     * @brief Flag determining if paths are to be drawn.
     */
    bool m_DrawPaths = false;
    
    /**
     * @brief Flag determining if postition points are to be drawn.
     */
    bool m_DrawPoints = false;

    /**
     * @brief The distance from the far clipping plane to the camera.
     */
    float m_Far = 200;

    /**
     * @brief The frame of data for which positions are being drawn.
     */
    int m_Frame = 0;

    /**
     * @brief True if panning is occurring, false otherwise.
     */
    bool m_IsPanning;

    /**
     * @brief True if rotation is occurring, false otherwise.
     */
    bool m_IsRotating;

    /**
     * @brief The X position of the mouse at the beginning of a movement event.
     */
    float m_LastX;

    /**
     * @brief The Y position of the mouse at the beginning of a movement event.
     */
    float m_LastY;

    /**
     * @brief The object controling the movement of the diffuse lighting source.
     */
    Camera3D m_LightingMatrix;

    /**
     * @brief The maximum path length to be drawn, as a fraction of total path
     * length range.
     */
    float m_MaxPathLength = 1;

    /**
     * @brief The minimum path length to be drawn, as a fraction of total path
     * length range.
     */
    float m_MinPathLength = 0;

    /**
     * @brief The uniform location within the shader files of the model to
     * world transformation matrix.
     */
    int m_ModelToWorld;

    /**
     * @brief The distance from the camera to the near clipping plane.
     */
    float m_Near = 1;

    /**
     * @brief The shader program used for drawing paths.
     */
    QOpenGLShaderProgram* m_PathProgram;

    /**
     * @brief The shader program used for drawing points.
     */
    QOpenGLShaderProgram* m_PointProgram;

    /**
     * @brief The projection matrix to be used.
     */
    QMatrix4x4 m_Projection;
    
    /**
     * @brief The total number of frames in the data.
     */
    int m_TotalFrames;

    /**
     * @brief The buffer in which vertices used for drawing points and paths
     * are stored.
     */
    QOpenGLBuffer m_TrajBuffer;
    
    /**
     * @brief The Transform3D object to be used for handling transformations.
     */
    Transform3D m_Transform;

    /**
     * @brief The @Vertex objects for each @Atom at each time step of the data.
     */
    QVector<QVector<Vertex>> m_Vertices;

    /**
     * @brief The uniform location within the shader files of the world to
     * camera transformation matrix.
     */
    int m_WorldToCamera;

    /**
     * @brief The zoom to be applied to the image, as a fraction.
     */
    float m_Zoom = 1.0;

    /**
     * @brief A scaling factor used when determining the appropriate far
     * clipping plane for the simulation space size.
     */
    const int FAR_SCALING = 3;

    /**
     * @brief The FOV used in gluPerspective(), in radians.
     */
    const float FOV = 0.88;

    /**
     * @brief The number one hundred as an int.
     */
    const int HUNDRED = 100;

    /**
     * @brief A scaling factor used when setting the radius of circles to be
     * drawn.
     */
    const float RADIUS_SCALING = 10.0;
    
    /**
     * @brief Scaling factor influencing the speed at which rotation occurs.
     */
    const float ROT_SPEED = 0.5;

    /**
     * @brief The square root of 3, used in calculating circle vertices.
     */
    const float SQRT_THREE = 1.732;

    /**
     * @brief Scaling factor influencing the speed at which translation occurs.
     */
    const float TRANS_SPEED = 0.4;
    
    /**
     * @brief The number of spatial dimensions being used.
     */
    const int TUPLE_SIZE_3D = 3;

    /**
     * @brief The rate at which zooming occurs.
     */
    const float ZOOM_SPEED = 1.35;
};

#endif // MYOPENGLWIDGET_H
