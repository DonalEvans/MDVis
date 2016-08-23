#include "ColourLegend.h"
#include <QPainter>

QColor ColourLegend::GetMax()
{
    return m_MaxColour;
}

void ColourLegend::SetMax(QColor colour)
{
    m_MaxColour = colour;
    update();
}

QColor ColourLegend::GetMin()
{
    return m_MinColour;
}

void ColourLegend::SetMin(QColor colour)
{
    m_MinColour = colour;
    update();
}

ColourLegend::ColourLegend(QWidget* parent)
{

}

void ColourLegend::paintEvent(QPaintEvent *event)
{
    QLinearGradient gradient(0, 0, this->width(), this->height());
    gradient.setColorAt(0.0, m_MaxColour);
    gradient.setColorAt(1.0, m_MinColour);
    QBrush brush(gradient);

    QPainter painter(this);
    painter.setPen(Qt::transparent);
    painter.setBrush(brush);
    painter.drawRect(0, 0, this->width(), this->height());
}
