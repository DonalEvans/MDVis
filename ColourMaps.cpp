#include "ColourMaps.h"

ColourMaps::ColourMaps()
{

}

QVector3D ColourMaps::GetColour(int map, int mapIndex)
{
    if (mapIndex >= GetMap(map).length())
    {
        return GetMap(map).last();
    }
    else if (mapIndex < 0)
    {
        return GetMap(map).first();
    }
    else
    {
        return GetMap(map)[mapIndex];
    }
}

QVector<QVector3D> ColourMaps::GetMap(int mapNum)
{
    if (mapNum >= m_ColourMaps.length())
    {
        return m_ColourMaps.last();
    }
    else if (mapNum < 0)
    {
        return m_ColourMaps.first();
    }
    else
    {
        return m_ColourMaps[mapNum];
    }
}

int ColourMaps::GetNumberOfMaps()
{
    return m_ColourMaps.length();
}
