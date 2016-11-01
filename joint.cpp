#include <QDebug>
#include "joint.h"

Joint::Joint()
{
    m_offset = -1;
}

bool Joint::isEnabled() const
{
    return (m_offset >= 0);
}

void Joint::setOffset(int offset)
{
    m_offset = offset;
}

void Joint::addJointAngle (double ts, QStringList values)
{
    // create a new joint angle
    double angle = values.at(m_offset).toDouble();

    qDebug() << "loading joint angle time=" << ts << " angle=" << angle;

    JointAngle* ja = new JointAngle ();
    ja->setAngle(ts, angle);
    m_jointAngles.append(ja);
}

JointAngle Joint::getAngleAt(int index) const
{
    if (index <= 0 || index >= m_jointAngles.size())
        index = 0;
    JointAngle* pja = m_jointAngles.at(0);
    // Avoid making a copy
    JointAngle& ja = *pja;
    return (ja);
}
