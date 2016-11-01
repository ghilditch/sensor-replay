#ifndef JOINT_H
#define JOINT_H

#include <QList>
#include <QStringList>

#include "jointangle.h"

class Joint
{

public:
    Joint();

    bool isEnabled() const;
    void setOffset(int offset);

    void addJointAngle (double ts, QStringList values);
    JointAngle getAngleAt(int index) const;

private:
    int m_offset;
    QList<JointAngle *> m_jointAngles;

};

#endif // JOINT_H
