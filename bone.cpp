#include <QDebug>
#include "bone.h"

Bone::Bone()
{
    m_offset = -1;
}

bool Bone::isEnabled() const
{
    return (m_offset >= 0);
}

void Bone::setOffset(int offset)
{
    m_offset = offset;
}

void Bone::addPosition(double ts, QStringList values)
{
    double x = values.at(m_offset).toDouble();
    double y = values.at(m_offset+1).toDouble();
    double z = values.at(m_offset+2).toDouble();

    //qDebug() << "loading bone position time=" << ts << " x=" << x << " y=" << y << " z=" << z;
    BonePosition* bp = new BonePosition ();
    bp->setPosition(ts, x, y, z);
    m_bonePositions.append(bp);
}

void Bone::addOrientation(double ts, QStringList values)
{
    double q1 = values.at(m_offset).toDouble();
    double q2 = values.at(m_offset+1).toDouble();
    double q3 = values.at(m_offset+2).toDouble();
    double q4 = values.at(m_offset+3).toDouble();

    //qDebug() << "loading bone orientation time=" << ts << " q1=" << q1 << " q2=" << q2 << " q3=" << q3 << " q4=" << q4;

    BoneOrientation* bo = new BoneOrientation();
    bo->setQuaternion(ts, q1, q2, q3, q4);
    m_boneOrientations.append(bo);
}

BonePosition Bone::getPositionAt(int index) const
{
    if (index <= 0 || index >= m_bonePositions.size())
        index = 0;

    BonePosition* pbp = m_bonePositions.at(index);
    // Avoid making a copy
    BonePosition& bp = *pbp;
    return (bp);
}

BoneOrientation Bone::getOrientationAt(int index) const
{
    if (index <= 0 || index >= m_boneOrientations.size())
        index = 0;
    BoneOrientation* pbo = m_boneOrientations.at(0);
    // Avoid making a copy
    BoneOrientation& bo = *pbo;
    return (bo);
}
