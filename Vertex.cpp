#include "Vertex.h"

QVector3D Vertex::GetPosition()
{
    return m_Position;
}

QVector3D Vertex::GetColour()
{
    return m_Colour;
}

Vertex::Vertex()
{

}

Vertex::Vertex(QVector3D position, QVector3D colour)
{
    m_Position = position;
    m_Colour = colour;
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

int Vertex::Stride()
{
   return sizeof(Vertex);
}
