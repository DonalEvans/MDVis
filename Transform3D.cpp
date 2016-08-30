#include "Transform3D.h"

QVector3D Transform3D::GetTranslation()
{
    return m_Translation;
}

void Transform3D::SetTranslation(QVector3D translation)
{
    m_Changed = true;
    m_Translation = translation;
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

QQuaternion Transform3D::GetRotation()
{
    return m_Rotation;
}

void Transform3D::SetRotation(QQuaternion rotation)
{
    m_Changed = true;
    m_Rotation = rotation;
}

Transform3D::Transform3D()
{

}

void Transform3D::Translate(QVector3D translate)
{
    m_Changed = true;
    SetTranslation(m_Translation + translate);
}

void Transform3D::Scale(QVector3D scale)
{
    m_Changed = true;
    SetScale(m_Scale * scale);
}

void Transform3D::Rotate(QQuaternion rotation)
{
    m_Changed = true;
    SetRotation(m_Rotation * rotation);
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
