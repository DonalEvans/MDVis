#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

class Vertex
{
public:
    QVector3D GetColour();

    QVector3D GetPosition();

    Vertex();

    Vertex(QVector3D position, QVector3D colour);

    ~Vertex();

    static int ColourOffset();

    static int PositionOffset();

    static int Stride();

    static const int TUPLE_SIZE = 3;

private:
    QVector3D m_Position;
    QVector3D m_Colour;
};

#endif // VERTEX_H
