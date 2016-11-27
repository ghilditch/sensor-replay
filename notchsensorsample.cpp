#include "notchsensorsample.h"
#include "vector.h"

NotchSensorSample::NotchSensorSample(QObject *parent) : QObject(parent){
    m_angle = 0;
}

QString NotchSensorSample::sensorName() const{
    return m_sensorName;
}

void NotchSensorSample::setSensorName(const QString &s){
    m_sensorName = s;
}

int NotchSensorSample::index() const{
    return m_index;
}

void NotchSensorSample::setIndex( int index){
    m_index = index;
}

double NotchSensorSample::timestamp() const{
    return m_timestamp;
}

void NotchSensorSample::setTimestamp(double t){
    m_timestamp = t;
}

BonePosition NotchSensorSample::position() const{
    return m_position;
}

void NotchSensorSample::setPosition(const BonePosition &p){
    m_position = p;
}

BoneOrientation NotchSensorSample::orientation() const{
    return m_orientation;
}

void NotchSensorSample::setOrientation(const BoneOrientation &o){
    m_orientation = o;
}

int NotchSensorSample::angle() const{
    return m_angle;
}

void NotchSensorSample::setAngle(const int a){
    m_angle = a;
    // Auto update the radian
    setRadian(Vector::degToRad(a));
}

qreal NotchSensorSample::radian() const{
    return m_radian;
}

void NotchSensorSample::setRadian(const qreal r){
    m_radian = r;
}

qreal NotchSensorSample::q1() const{
    return m_orientation.q1();
}

qreal NotchSensorSample::q2() const{
    return m_orientation.q2();
}

qreal NotchSensorSample::q3() const{
    return m_orientation.q3();
}

qreal NotchSensorSample::q4() const{
    return m_orientation.q4();
}

qreal NotchSensorSample::pos_x() const{
    return m_position.pos_x();
}

qreal NotchSensorSample::pos_y() const{
    return m_position.pos_y();
}

qreal NotchSensorSample::pos_z() const{
    return m_position.pos_z();
}
