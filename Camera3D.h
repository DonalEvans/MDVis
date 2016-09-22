/**
 * @file Camera3D.h
 * @author Donal Evans
 * @date 31 Aug 2016
 * @brief This class handles Camera3D movement.
 *
 * Detailed description goes here
 */

#ifndef CAMERA3D_H
#define CAMERA3D_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Camera3D
{
public:
    /**
     * @brief Setter for the default view matrix.
     * @param defaultView The QMatrix4x4 to be used as the default view.
     */
    void SetDefaultView(QMatrix4x4 defaultView);

    /**
     * @brief Setter for the transformation matrix.
     * @param matrix The new value for the transformation matrix.
     */
    void SetMatrix(QMatrix4x4 matrix);

    /**
     * @brief Getter for the rotation QQuaternion.
     * @return The current rotation as a QQuaternion.
     */
    QQuaternion GetRotation();

    /**
     * @brief Setter for the rotation QQuaternion.
     * @param scale A QQuaternion representing a rotation.
     */
    void SetRotation(QQuaternion rotation);

    /**
     * @brief Getter for the translation QVector3D.
     * @return The current translation as a QVector3D.
     */
    QVector3D GetTranslation();

    /**
     * @brief Setter for the translation QVector3D.
     * @param translation A QVector3D representing a translation.
     */
    void SetTranslation(QVector3D translation);

    /**
     * @brief Constructor
     */
    Camera3D();

    /**
     * @brief A function for determining the current forward direction.
     * @return A QVector3D representing the transformed forward direction.
     */
    QVector3D Forward();

    /**
     * @brief Sets the rotation quaternion to the identity quaternion.
     */
    void ResetRotation();

    /**
     * @brief Sets the translation to zero.
     */
    void ResetTranslation();

    /**
     * @brief Sets the camera position and orientation to the default.
     */
    void ResetView();

    /**
     * @brief A function for determining the current right direction.
     * @return A QVector3D representing the transformed right direction.
     */
    QVector3D Right();

    /**
     * @brief Applies a rotation to the existing rotation quaternion.
     * @param rotation A QQuaternion representing a rotation.
     */
    void Rotate(QQuaternion rotation);

    /**
     * @brief Converts the translation and rotation vectors into a QMatrix4x4.
     * @return A QMatrix4x4 representing the total 3D transformation.
     */
    QMatrix4x4 ToMatrix();

    /**
     * @brief Applies a translation to the existing translate vector.
     * @param translate A QVector3D representing a translation.
     */
    void Translate(QVector3D translate);

    /**
     * @brief A function for determining the current up direction.
     * @return A QVector3D representing the transformed up direction.
     */
    QVector3D Up();

    /**
     * @brief The default forward direction.
     */
    static const QVector3D FORWARD;

    /**
     * @brief The default right direction.
     */
    static const QVector3D RIGHT;

    /**
     * @brief The default up direction.
     */
    static const QVector3D UP;

  private:
    /**
     * @brief True if the transformation matrix has been changed since the last
     * time ToMatrix() was called, false otherwise.
     */
    bool m_Changed;

    /**
     * @brief Matrix describing the default camera view.
     */
    QMatrix4x4 m_DefaultView;

    /**
     * @brief The current rotation to be applied.
     */
    QQuaternion m_Rotation;

    /**
     * @brief The current translation to be applied.
     */
    QVector3D m_Translation;

    /**
     * @brief The total 3D transformation to be applied.
     */
    QMatrix4x4 m_World;
};

#endif // CAMERA3D_H
