#include <QDebug>
#include "bone.h"
#include "descriptivestatistics.h"

Bone::Bone(const QString &name): m_name(name), m_parent(NULL){
    m_offset = -1;
    m_angleOffset = -1;
    m_enabled = false;
}

Bone::~Bone(){
     foreach (NotchSensorSample* s, m_samples) {
        delete s;
     }
}

void Bone::setEnabled(bool b){
    m_enabled = b;
}

bool Bone::isEnabled() const{
    return m_enabled;
}


bool Bone::isDataCaptured() const{
    return (m_offset != -1);
}

bool Bone::hasData() const{
    return (m_samples.count() > 0);
}

bool Bone::isAngleEnabled() const{
    return (m_angleOffset >= 0);
}

void Bone::setOffset(int offset){
    m_offset = offset;
}

void Bone::setAngleOffset(int offset){
    m_angleOffset = offset;
}

QString Bone::name() const{
    return m_name;
}

Bone* Bone::parentBone() const{
    return m_parent;
}

double Bone::massRatio() const{
    return m_massRatio;
}

Vector Bone::vector() const{
    return m_vector;
}

int Bone::getSampleCount() const {
    return m_samples.count();
}

NotchSensorSample* Bone::getSampleAt(int index) const{
    if (index <= 0 || index >= m_samples.count())
        index = 0;

    NotchSensorSample* s = m_samples.at(index);
    return (s);
}

void Bone::setName(const QString&n){
    m_name = n;
}

void Bone::setParentBone(Bone* b){
    m_parent = b;
}

void Bone::setMassRatio(double mr){
    m_massRatio = mr;
}

void Bone::setVector(const Vector& v){
    m_vector = v;
}

void Bone::addPosition(double ts, QStringList values)
{
    double x = values.at(m_offset).toDouble();
    double y = values.at(m_offset+1).toDouble();
    double z = values.at(m_offset+2).toDouble();

    //qDebug() << "loading bone position time=" << ts << " x=" << x << " y=" << y << " z=" << z;

    // Create a new sample
    NotchSensorSample* s = new NotchSensorSample();
    s->setSensorName(m_name);
    s->setTimestamp(ts);

    // Create the position
    BonePosition bp;
    // Update the position
    bp.setPosition(x, y, z);
    // Update the sample
    s->setPosition(bp);

    // Add to list
    m_samples.append(s);

    // Set the index
    s->setIndex(m_samples.count());
}

void Bone::addOrientation(int index, QStringList values){
    double q1 = values.at(m_offset).toDouble();
    double q2 = values.at(m_offset+1).toDouble();
    double q3 = values.at(m_offset+2).toDouble();
    double q4 = values.at(m_offset+3).toDouble();

    //qDebug() << "loading bone orientation index=" << index << " q1=" << q1 << " q2=" << q2 << " q3=" << q3 << " q4=" << q4;

    BoneOrientation bo;
    bo.setQuaternion(q1, q2, q3, q4);

    // Get the sample
    if (m_samples.count() > 0 && index < m_samples.count()){
        // Update the sample
        NotchSensorSample* s = m_samples.at(index);
        s->setOrientation(bo);
    }
}

void Bone::setAngle(int index, int angle){
    if (m_samples.count() > 0 && index < m_samples.count()){
        // Update the sample
        NotchSensorSample* s = m_samples.at(index);
        s->setAngle(angle);
    }
    else
    {
        // Add new sample
        NotchSensorSample* s = new NotchSensorSample();
        s->setSensorName(m_name);
        s->setTimestamp(0);

        // Create the position
        BonePosition bp;
        // Update the position
        bp.setPosition(0, 0, 0);
        BoneOrientation bo;
        bo.setQuaternion(1, 0, 0, 0);
        s->setOrientation(bo);
        // Update the sample
        s->setPosition(bp);
        s->setAngle(angle);

        // Add to list
        m_samples.append(s);
    }
}

void Bone::addAngle(int index, QStringList values){
    // create a new joint angle
    double angle = values.at(m_angleOffset).toDouble();

    // Set direction depending on the bone
    if (m_name.indexOf("Arm") >= 0){
        angle *= -1;
    }

    //qDebug() << "loading joint bone=" << m_name << " index=" << index << " angle=" << angle;

    // Get the sample
    if (m_samples.count() > 0 && index < m_samples.count()){
        // Update the sample
        NotchSensorSample* s = m_samples.at(index);
        s->setAngle((int)angle);
    }
}

void Bone::applyRollingMeanFilter(){
    // Create a descriptive statistics filter
    DescriptiveStatistics stats;
    // Set the window size (this is hard-coded to 40 samples per second)
    stats.setWindowSize(40);

    // Update the radians as we smooth the buffer
    for (int i = 0; i < angles.count(); i++) {
        // add to our stats
        stats.addValue(s->radian());
        // Get the current mean value of the window
        double mean = stats.getMean();
        // Replace the radian
        m_samples.at(i)->setRadian(mean);
    }

}
