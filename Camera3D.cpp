#include "Camera3D.h"

void Camera3D::SetDefaultView(QMatrix4x4 defaultView)
{
    m_DefaultView = defaultView;
}

void Camera3D::SetMatrix(QMatrix4x4 matrix)
{
    m_World = matrix;
}

QQuaternion Camera3D::GetRotation()
{
    return m_Rotation;
}

void Camera3D::SetRotation(QQuaternion rotation)
{
    m_Changed = true;
    m_Rotation = rotation;
}

QVector3D Camera3D::GetTranslation()
{
    return m_Translation;
}

void Camera3D::SetTranslation(QVector3D translation)
{
    m_Changed = true;
    m_Translation = translation;
}

Camera3D::Camera3D()
{

}

const QVector3D Camera3D::FORWARD(0,0,-1);

QVector3D Camera3D::Forward()
{
    return m_Rotation.rotatedVector(FORWARD);
}

void Camera3D::ResetRotation()
{
    QQuaternion zero(1,0,0,0);
    SetRotation(zero);
}

void Camera3D::ResetTranslation()
{
    QVector3D zero(0,0,0);
    SetTranslation(zero);
}

void Camera3D::ResetView()
{
    ResetRotation();
    ResetTranslation();
}

const QVector3D Camera3D::RIGHT(1,0,0);

QVector3D Camera3D::Right()
{
    return m_Rotation.rotatedVector(RIGHT);
}

void Camera3D::Rotate(QQuaternion rotation)
{
    m_Changed = true;
    SetRotation(rotation * m_Rotation);
}

QMatrix4x4 Camera3D::ToMatrix()
{
    if (m_Changed)
    {
      m_Changed = false;
      m_World.setToIdentity();
      m_World *= m_DefaultView;
      m_World.rotate(m_Rotation.conjugate());
      m_World.translate(-m_Translation);
    }
    return m_World;
}

void Camera3D::Translate(QVector3D translate)
{
    m_Changed = true;
    SetTranslation(m_Translation + translate);
}

const QVector3D Camera3D::UP(0,1,0);

QVector3D Camera3D::Up()
{
    return m_Rotation.rotatedVector(UP);
}

