/**
 * @file Transform3D.h
 * @author Donal Evans
 * @date 26 Aug 2016
 * @brief This class generates a 4x4 transformation matrix.
 *
 * Detailed description goes here
 */

#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Transform3D
{
public:
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
     * @brief Getter for the scaling QVector3D.
     * @return The current scaling as a QVector3D.
     */
    QVector3D GetScale();

    /**
     * @brief Setter for the scaling QVector3D.
     * @param scale A QVector3D representing a scaling.
     */
    void SetScale(QVector3D scale);

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
    Transform3D();

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
     * @brief Applies a scaling to the existing scaling vector.
     * @param scale A QVector3D representing a scaling.
     */
    void Scale(QVector3D scale);

    /**
     * @brief Converts the translation, scaling and rotation vectors into
     * a QMatrix4x4.
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
     * @brief The current rotation to be applied.
     */
    QQuaternion m_Rotation;

    /**
     * @brief The current scaling to be applied.
     */
    QVector3D m_Scale{1,1,1};

    /**
     * @brief The current translation to be applied.
     */
    QVector3D m_Translation;

    /**
     * @brief The total 3D transformation to be applied.
     */
    QMatrix4x4 m_World;
};

#endif // TRANSFORM3D_H
