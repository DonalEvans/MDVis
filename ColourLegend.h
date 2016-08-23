#ifndef COLOURLEGEND_H
#define COLOURLEGEND_H

#include <QWidget>
#include <QColor>

class ColourLegend : public QWidget
{
    Q_OBJECT

public:
    QColor GetMax();

    void SetMax(QColor colour);

    QColor GetMin();

    void SetMin(QColor colour);

    ColourLegend(QWidget* parent);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor m_MaxColour = Qt::white;

    QColor m_MinColour = Qt::white;
};

#endif // COLOURLEGEND_H
