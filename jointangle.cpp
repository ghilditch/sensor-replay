#include "jointangle.h"

JointAngle::JointAngle(QObject *parent) : QObject(parent)
{

}

JointAngle::JointAngle(const JointAngle& copy)
{
    m_time_offset = copy.time_offset();
    m_angle = copy.angle();
}

qreal JointAngle::time_offset() const
{
    return m_time_offset;
}

qreal JointAngle::angle() const
{
    return m_angle;
}

void JointAngle::setAngle(qreal ts, qreal a)
{
    m_time_offset = ts;
    m_angle = a;
}
