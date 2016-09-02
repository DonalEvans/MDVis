#include "Vertex.h"
#include <QTextStream>

QVector3D Vertex::GetColour()
{
    return m_Colour;
}

void Vertex::SetColour(QVector3D colour)
{
    m_Colour = colour;
}

QVector3D Vertex::GetPosition()
{
    return m_Position;
}

void Vertex::SetPosition(QVector3D position)
{
    m_Position = position;
}

Vertex::Vertex()
{

}

Vertex::Vertex(QVector3D position, QVector3D colour)
{
    SetPosition(position);
    SetColour(colour);
}

Vertex::~Vertex()
{

}

int Vertex::ColourOffset()
{
    return offsetof(Vertex, m_Colour);
}

int Vertex::PositionOffset()
{
    return offsetof(Vertex, m_Position);
}

void Vertex::PrintVertex()
{
    QTextStream* out = new QTextStream(stdout, QIODevice::WriteOnly);
    *out << "Position = " << m_Position.x() << ", " << m_Position.y() << ", " << m_Position.z() << endl;
    *out << "Colour = " << m_Colour.x() << ", " << m_Colour.y() << ", " << m_Colour.z() << endl;
    delete out;
}

int Vertex::Stride()
{
   return sizeof(Vertex);
}
