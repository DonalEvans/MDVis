#include "ColourLegend.h"
#include <QPainter>
#include <QVector3D>
#include <QTextStream>

void ColourLegend::SetColourMap(QVector<QVector3D> map)
{
    m_ColourMap = map;
    update();
}

void ColourLegend::SetIsHorizontal(bool horizontal)
{
    m_IsHorizontal = horizontal;
}

QColor ColourLegend::GetMax()
{
    return m_MaxColour;
}

void ColourLegend::SetMax(QColor colour)
{
    m_MaxColour = colour;
    update();
}

QColor ColourLegend::GetMid()
{
    return m_MidColour;
}

void ColourLegend::SetMid(QColor colour)
{
    m_MidColour = colour;
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
    QLinearGradient gradient;
    if (m_IsHorizontal)
    {
        gradient.setStart(this->rect().topLeft());
        gradient.setFinalStop(this->rect().topRight());
    }
    else
    {
        gradient.setStart(this->rect().topLeft());
        gradient.setFinalStop(this->rect().bottomLeft());
    }
    QTextStream out(stdout);
    QColor colour;
    QVector3D vector;
    for (int i = 0; i < m_ColourMap.length(); ++i)
    {
        vector = m_ColourMap[i];
        colour = QColor::fromRgbF(vector.x(),vector.y(),vector.z());
        gradient.setColorAt((float)i/m_ColourMap.length(),colour);
    }
    QBrush brush(gradient);
    QPainter painter(this);
    painter.setPen(Qt::transparent);
    painter.setBrush(brush);
    painter.eraseRect(this->rect());
    painter.drawRect(this->rect());
}
