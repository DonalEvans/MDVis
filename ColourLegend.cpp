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

ColourLegend::ColourLegend(QWidget* parent)
{

}

void ColourLegend::paintEvent(QPaintEvent *event)
{
    QLinearGradient gradient;
    if (m_IsHorizontal)
    {
        gradient.setStart(this->rect().topRight());
        gradient.setFinalStop(this->rect().topLeft());
    }
    else
    {
        gradient.setStart(this->rect().bottomLeft());
        gradient.setFinalStop(this->rect().topLeft());
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
