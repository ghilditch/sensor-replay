#include "notchsensorsample.h"

NotchSensorSample::NotchSensorSample(QObject *parent) : QObject(parent)
{

}

QString NotchSensorSample::sensorName() const
{
    return m_sensorName;
}

void NotchSensorSample::setSensorName(const QString &s)
{
    m_sensorName = s;
}

QDateTime NotchSensorSample::timestamp() const
{
    return m_timestamp;
}

void NotchSensorSample::setTimestamp(const QDateTime &t)
{
    m_timestamp = t;
}

BonePosition NotchSensorSample::position() const
{
    return m_position;
}

void NotchSensorSample::setPosition(const BonePosition &p)
{
    m_position = p;
}

BoneOrientation NotchSensorSample::orientation() const
{
    return m_orientation;
}

void NotchSensorSample::setOrientation(const BoneOrientation &o)
{
    m_orientation = o;
}

qreal NotchSensorSample::angle() const
{
    return m_angle;
}

void NotchSensorSample::setAngle(const qreal a)
{
    m_angle = a;
}
