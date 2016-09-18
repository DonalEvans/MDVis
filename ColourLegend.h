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
    void SetColourMap(QVector<QVector3D> map);

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

    QVector<QVector3D> m_ColourMap;

    bool m_IsHorizontal = true;
};

#endif // COLOURLEGEND_H
