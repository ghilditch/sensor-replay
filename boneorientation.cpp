#include "boneorientation.h"

BoneOrientation::BoneOrientation( QObject *parent) : QObject(parent)
{

}

BoneOrientation::BoneOrientation(const BoneOrientation &copy)
{
    m_time_offset = copy.time_offset();
    m_q1 = copy.q1();
    m_q2 = copy.q2();
    m_q3 = copy.q3();
    m_q4 = copy.q4();
}

qreal BoneOrientation::time_offset() const
{
    return m_time_offset;
}

qreal BoneOrientation::q1() const
{
    return m_q1;
}

qreal BoneOrientation::q2() const
{
    return m_q2;
}

qreal BoneOrientation::q3() const
{
    return m_q3;
}

qreal BoneOrientation::q4() const
{
    return m_q4;
}

void BoneOrientation::setQuaternion(qreal ts, qreal q1, qreal q2, qreal q3, qreal q4)
{
    m_time_offset = ts;
    m_q1 = q1;
    m_q2 = q2;
    m_q3 = q3;
    m_q4 = q4;
}
