/**
 * @file ColourLegend.h
 * @author Donal Evans
 * @date 19 Aug 2016
 * @brief This class provides a widget painted with a colour gradient
 * for use as a legend.
 *
 * Detailed description goes here
 */

#ifndef COLOURLEGEND_H
#define COLOURLEGEND_H

#include <QWidget>
#include <QColor>
#include <QVector>
#include <QVector3D>

class ColourLegend : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Sets the colour map for this object to the one in map.
     * @param map A QVector of colours, as QVector3D RBG float values, from 0-1.
     */
    void SetColourMap(QVector<QVector3D> map);

    /**
     * @brief Sets the flag for whether or not the colour gradient on this
     * object is horizontal.
     * @param horizontal True if the colour gradient on the object is
     * horizontal, false if vertical.
     */
    void SetIsHorizontal(bool horizontal);

    /**
     * @brief Constructor
     * @param parent The parent QWidget of this widget.
     */
    ColourLegend(QWidget* parent);

protected:
    /**
     * @brief Function for painting the colour gradient to the widget.
     * @param event The QPaintEvent that triggered painting.
     */
    void paintEvent(QPaintEvent *event);

private:

    /**
     * @brief The colour map to be used to create the colour gradient, as a
     * QVector of QVector3D RBG float values, from 0-1.
     */
    QVector<QVector3D> m_ColourMap;

    /**
     * @brief Flag for whether or not the colour gradient will be horizontal.
     */
    bool m_IsHorizontal = true;
};

#endif // COLOURLEGEND_H
