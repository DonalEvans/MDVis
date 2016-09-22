/**
 * @file ColourMaps.h
 * @author Donal Evans
 * @date 09 Sep 2016
 * @brief This class provides a list of colour maps, as QVectors of QVector3D
 * RBG float values, from 0-1.
 *
 * Detailed description goes here
 */

#ifndef COLOURMAPS_H
#define COLOURMAPS_H

#include <QVector3D>
#include <QVector>

class ColourMaps
{
public:
    /**
     * @brief Constructor.
     */
    ColourMaps();

    /**
     * @brief Access function for an individual colour in a colour map.
     * @param map The map to be accessed.
     * @param mapIndex The index of the colour to be accessed.
     * @return A QVector3D of RBG float values, from 0-1.
     */
    QVector3D GetColour(int map, int mapIndex);

    /**
     * @brief Access function for a particular colour map.
     * @param mapNum The index of the colour map to be accessed.
     * @return A QVector of QVector3D RBG float values, from 0-1.
     */
    QVector<QVector3D> GetMap(int mapNum);

    /**
     * @brief Returns the length of m_ColourMaps.
     * @return The number of maps in m_ColourMaps.
     */
    int GetNumberOfMaps();

private:
    /**
     *@brief A QVector of colour maps, each one being a QVector of QVector3D
     *RBG float values, from 0-1.
     */
    QVector<QVector<QVector3D> > m_ColourMaps;
};
#endif // COLOURMAPS_H
