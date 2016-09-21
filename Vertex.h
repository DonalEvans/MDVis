/**
 * @file Vertex.h
 * @author Donal Evans
 * @date 25 Jul 2016
 * @brief This class stores position and colour information for one vertex.
 *
 * Detailed description goes here
 */

#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

class Vertex
{
public:
    /**
     * @brief Getter for the colour of the vertex.
     * @return A QVector3D representing the colour.
     */
    QVector3D GetColour();

    /**
     * @brief Setter for the colour of the vertex.
     * @param colour A QVector3D representing the colour.
     */
    void SetColour(QVector3D colour);

    /**
     * @brief Getter for the position of the vertex.
     * @return A QVector3D representing the position.
     */
    QVector3D GetPosition();

    /**
     * @brief Setter for the position of the vertex.
     * @param position A QVector3D representing the position.
     */
    void SetPosition(QVector3D position);

    /**
     * @brief Empty constructor.
     */
    Vertex();

    /**
     * @brief Constructor
     * @param position The position of the vertex.
     * @param colour The colour of the vertex.
     */
    Vertex(QVector3D position, QVector3D colour);

    /**
     * @brief Destructor.
     */
    ~Vertex();

    /**
     * @brief Returns the offset within a Vertex object of m_Colour
     * @return The offset, in bytes, of m_Colour within a Vertex
     */
    static int ColourOffset();

    /**
     * @brief Returns the offset within a Vertex object of m_Position
     * @return The offset, in bytes, of m_Position within a Vertex
     */
    static int PositionOffset();

    /**
     * @brief Prints the position and colour of the vertex.
     */
    void PrintVertex();

    /**
     * @brief Convenience function. Returns the size of a Vertex.
     * @return The size of a Vertex, in bytes.
     */
    static int Stride();

    /**
     * @brief The tuple size for the position and colour vectors.
     */
    static const int TUPLE_SIZE = 3;

private:
    /**
     * @brief The colour of the vertex.
     */
    QVector3D m_Colour;

    /**
     * @brief The position of the vertex.
     */
    QVector3D m_Position;
};

#endif // VERTEX_H
