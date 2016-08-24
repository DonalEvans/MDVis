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

class ColourLegend : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Getter for the colour representing the maximum value.
     * @return The colour as a QColor.
     */
    QColor GetMax();

    /**
     * @brief Setter for the colour representing the maximum value.
     * @param colour The colour as a QColor.
     */
    void SetMax(QColor colour);

    /**
     * @brief Getter for the colour representing the minimum value.
     * @return The colour as a QColor.
     */
    QColor GetMin();

    /**
     * @brief Setter for the colour representing the minimum value.
     * @param colour The colour as a QColor.
     */
    void SetMin(QColor colour);

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
     * @brief The colour representing the maximum value.
     */
    QColor m_MaxColour = Qt::white;

    /**
     * @brief The colour representing the minimum value.
     */
    QColor m_MinColour = Qt::white;
};

#endif // COLOURLEGEND_H
