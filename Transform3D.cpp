#include "Transform3D.h"

void Transform3D::SetMatrix(QMatrix4x4 matrix)
{
    ResetRotation();
    ResetTranslation();
    m_World = matrix;
    m_Changed = false;
}

QQuaternion Transform3D::GetRotation()
{
    return m_Rotation;
}

void Transform3D::SetRotation(QQuaternion rotation)
{
    m_Changed = true;
    m_Rotation = rotation;
}

QVector3D Transform3D::GetScale()
{
    return m_Scale;
}

void Transform3D::SetScale(QVector3D scale)
{
    m_Changed = true;
    m_Scale = scale;
}

QVector3D Transform3D::GetTranslation()
{
    return m_Translation;
}

void Transform3D::SetTranslation(QVector3D translation)
{
    m_Changed = true;
    m_Translation = translation;
}

Transform3D::Transform3D()
{

}

const QVector3D Transform3D::FORWARD(0,0,1);

QVector3D Transform3D::Forward()
{
    return m_Rotation.rotatedVector(FORWARD);
}

void Transform3D::ResetRotation()
{
    QQuaternion zero(1,0,0,0);
    SetRotation(zero);
}

void Transform3D::ResetTranslation()
{
    QVector3D zero(0,0,0);
    SetTranslation(zero);
}

const QVector3D Transform3D::RIGHT(1,0,0);

QVector3D Transform3D::Right()
{
    return m_Rotation.rotatedVector(RIGHT);
}

void Transform3D::Rotate(QQuaternion rotation)
{
    m_Changed = true;
    SetRotation(rotation * m_Rotation);
}

void Transform3D::Scale(QVector3D scale)
{
    m_Changed = true;
    SetScale(m_Scale * scale);
}

QMatrix4x4 Transform3D::ToMatrix()
{
    if (m_Changed)
    {
      m_Changed = false;
      m_World.setToIdentity();
      m_World.translate(m_Translation);
      m_World.rotate(m_Rotation);
      m_World.scale(m_Scale);
    }
    return m_World;
}

void Transform3D::Translate(QVector3D translate)
{
    m_Changed = true;
    SetTranslation(m_Translation + translate);
}

const QVector3D Transform3D::UP(0,1,0);

QVector3D Transform3D::Up()
{
    return m_Rotation.rotatedVector(UP);
}
